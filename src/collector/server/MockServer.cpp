#include "server/Server.hpp"
#include <iostream>


MockServer::MockServer(std::string type, std::string base, std::string quote, std::string exchange)
    : BaseServer(type, base, quote, exchange) {}

void MockServer::start() {
    while (true) {
        auto msg = generateRandomMessage(type, base, quote, exchange);
            auto payload_string = std::string(msg.payload, msg.payload_size);
            std::cout << "Mock Server sending message:" << std::endl;
            std::cout << "Type: " << msg.key.type << std::endl;
            std::cout << "Base: " << msg.key.base << std::endl;
            std::cout << "Quote: " << msg.key.quote << std::endl;
            std::cout << "Exchange: " << msg.key.exchange << std::endl;
            std::cout << "Timestamp: " << msg.timestamp << std::endl;
            std::cout << "Payload size: " << msg.payload_size << std::endl;
            std::cout << "Payload: " << payload_string << std::endl;
            std::cout << "-------------------" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Sleep for 1 second
    }
}
