#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <algorithm>

#define PORT 6667
#define BUFFER_SIZE 1024

class IRCServer {
public:
    IRCServer(int port);
    void run();

private:
    int server_fd;
    struct sockaddr_in address;
    std::vector<struct pollfd> poll_fds;

    void handleNewConnection();
    void handleClientMessage(int client_fd);
    void sendMessage(int client_fd, const std::string& message);
};

IRCServer::IRCServer(int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    poll_fds.push_back({server_fd, POLLIN, 0});
}

void IRCServer::run() {
    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);
        if (poll_count < 0) {
            perror("poll failed");
            break;
        }

        for (size_t i = 0; i < poll_fds.size(); ++i) {
            if (poll_fds[i].revents & POLLIN) {
                if (poll_fds[i].fd == server_fd) {
                    handleNewConnection();
                } else {
                    handleClientMessage(poll_fds[i].fd);
                }
            }
        }
    }

    close(server_fd);
}

void IRCServer::handleNewConnection() {
    int new_socket;
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);

    if ((new_socket = accept(server_fd, (struct sockaddr*)&client_address, &client_len)) < 0) {
        perror("accept failed");
        return;
    }

    std::cout << "New client connected" << std::endl;
    poll_fds.push_back({new_socket, POLLIN, 0});
}

void IRCServer::handleClientMessage(int client_fd) {
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE);

    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            std::cout << "Client disconnected" << std::endl;
        } else {
            perror("read failed");
        }
        close(client_fd);
        poll_fds.erase(std::remove_if(poll_fds.begin(), poll_fds.end(), [client_fd](pollfd& pfd) { return pfd.fd == client_fd; }), poll_fds.end());
    } else {
        std::cout << "Received message: " << buffer << std::endl;
        sendMessage(client_fd, buffer); // Echo the message back to the client
    }
}

void IRCServer::sendMessage(int client_fd, const std::string& message) {
    send(client_fd, message.c_str(), message.length(), 0);
}

int main() {
    IRCServer server(PORT);
    server.run();
    return 0;
}

