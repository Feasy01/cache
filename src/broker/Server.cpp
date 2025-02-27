#include "broker/Server.hpp"


Server::Server(int publisher_port, int subscriber_port)
    : pub_port(publisher_port), sub_port(subscriber_port), running(true) {
    
    subscription_root = std::make_unique<SubscriptionNode>();
    subscription_root->is_wildcard = false;
    
    pub_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    setupSocket(pub_socket_fd, pub_port);
    
    sub_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    setupSocket(sub_socket_fd, sub_port);
    
    connection_thread = std::thread(&Server::handleConnections, this);
    processor_thread = std::thread(&Server::processMessages, this);
}

Server::~Server() {
    std::cout << "Server destructor called" << std::endl;
    running = false;
    if (connection_thread.joinable()) {
        connection_thread.join();
    }
    if (processor_thread.joinable()) {
        processor_thread.join();
    }
    close(pub_socket_fd);
    close(sub_socket_fd);
}

void Server::setupSocket(int socket_fd, int port) {
    int opt = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));

    int flags = fcntl(socket_fd, F_GETFL, 0);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Socket binding failed");
    }
    std::cout << "Socket bound to port " << port << std::endl;
    if (listen(socket_fd, 10) < 0) {
        throw std::runtime_error("Socket listen failed");
    }
    std::cout << "Socket listening on port " << port << std::endl;
}

void Server::handleConnections() {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        throw std::runtime_error("Failed to create epoll instance");
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    
    event.data.fd = pub_socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pub_socket_fd, &event) == -1) {
        throw std::runtime_error("Failed to add publisher socket to epoll");
    }
    
    event.data.fd = sub_socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sub_socket_fd, &event) == -1) {
        throw std::runtime_error("Failed to add subscriber socket to epoll");
    }

    std::vector<struct epoll_event> events(64); 

    while (running) {
        int nfds = epoll_wait(epoll_fd, events.data(), events.size(), 1000);
        if (nfds == -1) {
            if (errno == EINTR) continue;  
            throw std::runtime_error("epoll_wait failed");
        }

        for (int n = 0; n < nfds; ++n) {
            int fd = events[n].data.fd;
            
            if (fd == pub_socket_fd) {
                acceptClient(pub_socket_fd, epoll_fd, true);
            } else if (fd == sub_socket_fd) {
                acceptClient(sub_socket_fd, epoll_fd, false);
            } else {
                handleClient(fd, events[n].events);
            }
        }
    }

    close(epoll_fd);
}

void Server::acceptClient(int socket_fd, int epoll_fd, bool is_publisher) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        return;
    }

    // Set non-blocking
    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;  // Edge-triggered mode
    event.data.fd = client_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
        close(client_fd);
        throw std::runtime_error("Failed to add client to epoll");
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        client_types[client_fd] = is_publisher;
    }
    
    std::cout << (is_publisher ? "Publisher" : "Subscriber") << " connected on fd: " << client_fd << std::endl;
}

void Server::handleClient(int client_fd, uint32_t events) {
    bool is_publisher;
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        auto it = client_types.find(client_fd);
        if (it == client_types.end()) {
            close(client_fd);
            return;
        }
        is_publisher = it->second;
    }

    if (events & EPOLLIN) {
        if (is_publisher) {
            handlePublisherMessage(client_fd);
        } else {
            handleSubscriberMessage(client_fd);
        }
    }

    if (events & (EPOLLHUP | EPOLLERR)) {
        removeClient(client_fd);
    }
}

void Server::handlePublisherMessage(int client_fd) {
    char buffer[2048];
    OBMessageDTO2 msg;
    
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        removeClient(client_fd);
        return;
    }

    memcpy(&msg, buffer, sizeof(OBMessageDTO2));
    {
        std::lock_guard<std::mutex> lock(message_mutex);
        message_queue.push(std::move(msg));
    }
    message_cv.notify_one();
}

void Server::handleSubscriberMessage(int client_fd) {
    char buffer[1024];
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        removeClient(client_fd);
        return;
    }

    // Handle initial subscription
    std::string pattern(buffer, bytes_read);
    std::vector<std::string> parts;
    std::stringstream ss(pattern);
    std::string part;
    while (std::getline(ss, part, '_')) {
        parts.push_back(part);
    }
    
    addSubscription(client_fd, parts);
}

void Server::removeClient(int client_fd) {
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        client_types.erase(client_fd);
    }
    close(client_fd);
    std::cout << "Client disconnected: " << client_fd << std::endl;
}

void Server::processMessages() {
    while (running) {
        std::unique_lock<std::mutex> lock(message_mutex);
        message_cv.wait(lock, [this]() { 
            return !message_queue.empty() || !running; 
        });
        
        if (!running) break;
        
        while (!message_queue.empty()) {
            auto message = message_queue.front();
            message_queue.pop();
            lock.unlock();
            std::cout << message.key.type << "_" << message.key.base << "_" << message.key.quote << "_" << message.key.exchange << std::endl;
            matchAndPublish(message);
            
            lock.lock();
        }
    }
}

void Server::matchAndPublish(const OBMessageDTO2& msg) {
    std::vector<int> matching_fds = findMatchingSubscribers(msg);
    for (int fd : matching_fds) {
        std::cout << "Sending message to fd: " << fd << std::endl;
        send(fd, &msg, sizeof(msg), 0);
    }
}

std::vector<int> Server::findMatchingSubscribers(const OBMessageDTO2& msg) {
    std::vector<int> result;
    std::function<void(SubscriptionNode*, const std::vector<std::string>&, size_t)> traverse;
    
    std::vector<std::string> msg_parts = {
        msg.key.type,
        msg.key.base,
        msg.key.quote,
        msg.key.exchange
    };
    std::cout << "\n=== Starting Traversal ===" << std::endl;
    std::cout << "Looking for: " << msg.key.type << "_" << msg.key.base << "_" 
              << msg.key.quote << "_" << msg.key.exchange << std::endl;
    
    traverse = [&](SubscriptionNode* node, const std::vector<std::string>& parts, size_t depth) {
        std::string indent(depth * 2, ' '); 
        std::cout << indent << "Traversing node with value: " << (node->value.empty() ? "ROOT" : node->value) 
                  << " (depth: " << depth << ")" << std::endl;
        
        if (!node->subscriber_fds.empty()) {
            std::cout << indent << "Found " << node->subscriber_fds.size() 
                      << " subscribers at current node" << std::endl;
            result.insert(result.end(), 
                         node->subscriber_fds.begin(), 
                         node->subscriber_fds.end());
        }
        
        if (depth < parts.size()) {
            std::cout << indent << "Looking for part: " << parts[depth] << std::endl;
            for (const auto& child : node->children) {
                std::cout << indent << "Checking child: " << child.first 
                          << " (wildcard: " << (child.second->is_wildcard ? "yes" : "no") << ")" << std::endl;
                if (child.second->is_wildcard || child.first == parts[depth]) {
                    std::cout << indent << "-> Matching child found, traversing deeper" << std::endl;
                    traverse(child.second.get(), parts, depth + 1);
                } else {
                    std::cout << indent << "-> No match, skipping" << std::endl;
                }
            }
        }
    };
    
    traverse(subscription_root.get(), msg_parts, 0);
    std::cout << "=== Traversal Complete ===\n" << std::endl;
    return result;
}

void Server::addSubscription(int fd, const std::vector<std::string>& pattern) {
    std::cout << "\n=== Adding New Subscription ===" << std::endl;
    std::cout << "Subscriber fd: " << fd << std::endl;
    std::cout << "Pattern: ";
    for (const auto& part : pattern) {
        std::cout << part << "_";
    }
    std::cout << std::endl;
    
    SubscriptionNode* current = subscription_root.get();
    std::cout << "Starting at root node" << std::endl;
    
    size_t depth = 0;
    for (const std::string& part : pattern) {
        std::string indent(depth * 2, ' ');
        bool is_wildcard = (part == "*");
        std::string key = is_wildcard ? "*" : part;
        
        std::cout << indent << "Processing part: " << key 
                  << " (wildcard: " << (is_wildcard ? "yes" : "no") << ")" << std::endl;
        
        if (current->children.find(key) == current->children.end()) {
            std::cout << indent << "-> Creating new node for key: " << key << std::endl;
            current->children[key] = std::make_unique<SubscriptionNode>();
            current->children[key]->value = key;
            current->children[key]->is_wildcard = is_wildcard;
        } else {
            std::cout << indent << "-> Node already exists for key: " << key << std::endl;
        }
        
        current = current->children[key].get();
        depth++;
    }
    
    current->subscriber_fds.insert(fd);
    std::cout << "Added fd " << fd << " to final node" << std::endl;
    std::cout << "=== Subscription Complete ===\n" << std::endl;
}