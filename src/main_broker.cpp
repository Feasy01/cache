#include "broker/Server.hpp"

int main() {
    Server server(9000, 9001);
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}