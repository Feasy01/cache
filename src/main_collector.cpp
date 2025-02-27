#include "server/Server.hpp"
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <base_currency> <quote_currency> <exchange_name> <port>" << std::endl;
        return 1;
    }

    std::string base_currency = std::string(argv[1]);
    std::string quote_currency = std::string(argv[2]);
    std::string exchange = std::string(argv[3]);
    std::string port = std::string(argv[4]);
    std::cout << "Starting collector with base: " << base_currency << " quote: " << quote_currency << " exchange: " << exchange << " port: " << port << std::endl;
    Server mockServer("0.0.0.0", port, "OB", base_currency, quote_currency, exchange);
    mockServer.start();
    return 0;
}