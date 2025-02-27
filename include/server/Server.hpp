#include "data_transfer_object/DataTransferObject.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <random>
#include <chrono>
#include <thread>
#include <sys/epoll.h>
#include "utils/Lz4Utils.hpp"



class BaseServer{
    public:
        BaseServer(std::string type, std::string base, std::string quote, std::string exchange );
        OBMessageDTO2 generateRandomMessage(std::string type, std::string base, std::string quote, std::string exchange);
        static constexpr std::array<std::array<char, 1024>, 100> generate_compile_time_payloads();
        static constexpr double compile_time_random(uint32_t seed, double min, double max);
        static constexpr uint32_t hash(uint32_t x);
        static constexpr size_t sprintf_double(char* str, double value);
        static constexpr size_t sprintf_constexpr(char* str, const char* format, size_t value, double fvalue);
        void start();
        std::string type;
        std::string base;
        std::string quote;
        std::string exchange;
};


class Server : public BaseServer{
    public:
        Server(std::string broker_ip, std::string broker_port, std::string type, std::string base, std::string quote, std::string exchange );
        ~Server();
        void start();
    private:
        int socket_fd;
        int client_fd;
        int epoll_fd;
        int port;
        std::string broker_ip;
        std::string broker_port;
};

class MockServer : public BaseServer {
    public:
        MockServer( std::string type, std::string base, std::string quote, std::string exchange);
        void start();

};


