#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "data_transfer_object/DataTransferObject.hpp"
#include <unordered_set>
#include <unordered_map>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <vector>
#include <sstream>
#include <functional>

struct SubscriptionNode {
    std::string value;
    bool is_wildcard;
    std::unordered_set<int> subscriber_fds;
    std::unordered_map<std::string, std::unique_ptr<SubscriptionNode>> children;
};

class Server {
public:
    Server(int publisher_port, int subscriber_port);
    ~Server();

private:
    int port;
    int socket_fd;
    bool running;
    int epoll_fd;
    void acceptClient(int socket_fd, int epoll_fd, bool is_publisher);
    void handleClient(int client_fd, uint32_t events);
    void handlePublisherMessage(int client_fd);
    void handleSubscriberMessage(int client_fd);
    void removeClient(int client_fd);
    std::unordered_map<int, bool> client_types;
    std::mutex clients_mutex;
    std::thread connection_thread;
    std::thread processor_thread;
    std::mutex message_mutex;
    std::condition_variable message_cv;
    std::queue<OBMessageDTO2> message_queue;

    int pub_port;
    int sub_port;
    int pub_socket_fd;
    int sub_socket_fd;
    std::unique_ptr<SubscriptionNode> subscription_root;

    void handleConnections();
    void processMessages();
    void acceptClient(int socket_fd, void (Server::*handle_client)(int));
    void setupSocket(int socket_fd, int port);
    void handlePublisher(int client_fd);
    void handleSubscriber(int client_fd);
    void matchAndPublish(const OBMessageDTO2& msg);
    void addSubscription(int fd, const std::vector<std::string>& pattern);
    std::vector<int> findMatchingSubscribers(const OBMessageDTO2& msg);
};