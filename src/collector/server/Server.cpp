#include "server/Server.hpp"



Server::Server(std::string broker_ip, std::string broker_port, std::string type, std::string base, std::string quote, std::string exchange) 
    : BaseServer(type, base, quote, exchange), broker_ip(broker_ip), broker_port(broker_port), client_fd(-1), epoll_fd(-1) {
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        throw std::runtime_error("Socket creation failed");
    }
    std::cout << "Socket created" << std::endl;

    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        throw std::runtime_error("Epoll creation failed");
    }
    std::cout << "Epoll created" << std::endl;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(std::stoi(broker_port));
    address.sin_addr.s_addr = htonl(std::stoi(broker_ip));

    if (connect(client_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Connection failed");
    }
    std::cout << "Connected to broker" << std::endl;
    struct epoll_event ev;
    ev.events = EPOLLOUT;  
    ev.data.fd = client_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
        throw std::runtime_error("Epoll control failed");
    }
    std::cout << "Epoll control added" << std::endl;
}

void Server::start() {
    struct epoll_event events[1];
    
    while (true) {
        int nfds = epoll_wait(epoll_fd, events, 1, -1); 
        
        if (nfds < 0) {
            throw std::runtime_error("Epoll wait failed: " + std::string(strerror(errno)));
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].events & EPOLLOUT) {
                auto msg = generateRandomMessage(type, base, quote, exchange);
                if (send(client_fd, &msg, sizeof(msg), 0) < 0) {
                    throw std::runtime_error("Send failed");
                }
            }
        }
    }
}

Server::~Server() {
    close(client_fd);
    close(epoll_fd);
}

