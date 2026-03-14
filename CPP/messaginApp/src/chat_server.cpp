#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <algorithm>
#include "chat_server.hpp"

ChatServer::ChatServer() : serverSocket(-1), isRunning(false) {}

ChatServer::~ChatServer() {
    stop();
}

void ChatServer::start(const std::string& ip, int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Fatal: Could not create server socket." << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr) <= 0) {
        std::cerr << "Fatal: Invalid IP address: " << ip << std::endl;
        return;
    }

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Fatal: Bind failed on " << ip << ":" << port << ". errno: " << errno << std::endl;
        return;
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Fatal: Listen failed." << std::endl;
        return;
    }

    isRunning = true;
    std::cout << "[*] ChatServer active on port " << port << ". Awaiting connections..." << std::endl;

    while (isRunning) {
        sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);
        int clientFD = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);

        if (clientFD < 0) {
            if (isRunning) std::cerr << "Warning: Accept failed." << std::endl;
            continue;
        }

        auto client = std::make_shared<Socket>(clientFD);
        client->setIdentity(clientAddr);

        std::cout << "[+] New Session: " << client->getIdentity() << std::endl;

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clientSockets.push_back(client);
        }

        // Use a lambda to correctly pass the shared_ptr and call the member function
        std::thread([this, client]() { this->handleClient(client); }).detach();
    }
}

void ChatServer::stop() {
    isRunning = false;
    if (serverSocket != -1) {
        close(serverSocket);
        serverSocket = -1;
    }
    std::cout << "[*] ChatServer shutting down..." << std::endl;
}

void ChatServer::handleClient(std::shared_ptr<Socket> client) {
    char buffer[1024];
    std::string accumulationBuffer;

    while (isRunning) {
        IoResult result = client->receive(buffer, sizeof(buffer));

        if (result.status != SocketStatus::SUCCESS) {
            if (result.status == SocketStatus::DISCONNECTED) {
                std::cout << "[-] " << client->getIdentity() << " disconnected." << std::endl;
            } else {
                std::cerr << "[!] Network error with " << client->getIdentity() << ". errno: " << errno << std::endl;
            }

            removeClient(client);
            break;
        }

        accumulationBuffer.append(buffer, result.bytes);

        size_t delimiterPos;
        while ((delimiterPos = accumulationBuffer.find('\n')) != std::string::npos) {
            std::string message = accumulationBuffer.substr(0, delimiterPos);
            
            std::cout << "[Relay] " << client->getIdentity() << " -> All: " << message << std::endl;
            broadcast(message, client);

            accumulationBuffer.erase(0, delimiterPos + 1);
        }
    }
}

void ChatServer::broadcast(const std::string& message, const std::shared_ptr<Socket>& sender) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    std::string formattedMsg = sender->getIdentity() + ": " + message + "\n";

    for (const auto& recipient : clientSockets) {
        if (recipient != sender) {
            recipient->send(formattedMsg);
        }
    }
}

void ChatServer::removeClient(const std::shared_ptr<Socket>& client) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    clientSockets.erase(
        std::remove(clientSockets.begin(), clientSockets.end(), client), 
        clientSockets.end()
    );
}
