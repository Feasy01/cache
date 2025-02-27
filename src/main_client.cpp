#include "client/Client.hpp"

int main() {
    Client client("0.0.0.0", "9001");
    client.connect_to_broker();
    client.start();
    return 0;
}