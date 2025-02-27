#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "utils/Lz4Utils.hpp"
#include "data_transfer_object/DataTransferObject.hpp"
class Client {
    public:
        Client(std::string ip, std::string port);
        bool connect_to_broker();
        void start();
    private:
        std::string ip;
        std::string port;
        int socket_fd;
        int epoll_fd;
        std::string broker_ip;
        std::string broker_port;
};