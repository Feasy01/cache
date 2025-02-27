#include "client/Client.hpp"

Client::Client(std::string ip, std::string port)
    : ip(ip), port(port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        close(socket_fd);
        throw std::runtime_error("Failed to create epoll");
    }
}

bool Client::connect_to_broker() {
    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(port));
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        return false;
    }

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        return false;
    }

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socket_fd;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) < 0) {
        return false;
    }

    return true;
}

void Client::start() {
    const int max_events = 1;
    char buffer[1440];
    epoll_event events[max_events];

    std::string matching_pattern = "*";
    if(write(socket_fd, matching_pattern.c_str(), matching_pattern.size()) < 0) {
        throw std::runtime_error("Failed to send handshake message");
    }
    std::cout << "Sent handshake message" << std::endl;


    while (true) {
        int num_events = epoll_wait(epoll_fd, events, max_events, -1);
        if (num_events == -1) {
            throw std::runtime_error("Failed to wait for events");
        }
        for (int i = 0; i < num_events; i++) {
            if (events[i].data.fd == socket_fd) {
                int bytes_read = read(socket_fd, buffer, sizeof(buffer));
                if (bytes_read == -1) {
                    throw std::runtime_error("Failed to read from socket");
                }
                std::cout << "Received message: " << std::string(buffer, bytes_read) << std::endl;
            }
        }
    }
}