#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "chat_client.hpp"

ChatClient::ChatClient() : clientSocket(nullptr), isRunning(false) {}

ChatClient::~ChatClient() {
    stop();
}

bool ChatClient::connect(const std::string& ip, int port) {
    int rawSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (rawSocket == -1) {
        std::cerr << "Failed to create a socket. errno: " << errno << std::endl;
        return false;
    }

    // Immediately wrap in our RAII Socket
    clientSocket = std::make_shared<Socket>(rawSocket);

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr) <= 0) {
        std::cerr << "Invalid IP address: " << ip << std::endl;
        return false;
    }

    if (::connect(rawSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to server at " << ip << ":" << port << ". errno: " << errno << std::endl;
        return false;
    }

    std::cout << "[*] Connected to server at " << ip << ":" << port << std::endl;
    isRunning = true;
    
    // Start background receive thread
    recvThread = std::thread(&ChatClient::receiveLoop, this);
    return true;
}

void ChatClient::run() {
    std::string send_message;
    std::cout << "[*] Type your message and press Enter. (Ctrl+D or empty line to exit)" << std::endl;

    while (isRunning) {
        std::cout << "Client: " << std::flush;
        if (!std::getline(std::cin, send_message)) break;

        if (send_message.empty()) {
            continue;
        }

        clientSocket->send(send_message + "\n");
    }

    stop();
}

void ChatClient::stop() {
    isRunning = false;
    if (clientSocket) {
        clientSocket->shutdownReadWrite();
    }
    if (recvThread.joinable()) {
        recvThread.join();
    }
    clientSocket.reset();
    std::cout << "[*] Disconnected from server." << std::endl;
}

void ChatClient::receiveLoop() {
    char buffer_recv[1024];
    std::string accumulationBuffer = "";

    while (isRunning) {
        IoResult result = clientSocket->receive(buffer_recv, sizeof(buffer_recv));

        if (result.status != SocketStatus::SUCCESS) {
            if (result.status == SocketStatus::DISCONNECTED) {
                std::cout << "\n[!] Connection lost: Server closed the connection." << std::endl;
            } else if (isRunning) {
                std::cerr << "\n[!] Error receiving from server. errno: " << errno << std::endl;
            }
            isRunning = false;
            break;
        }

        accumulationBuffer.append(buffer_recv, result.bytes);

        size_t delimiterPos;
        while ((delimiterPos = accumulationBuffer.find('\n')) != std::string::npos) {
            std::string complete_msg = accumulationBuffer.substr(0, delimiterPos);
            
            // UI Trick: Clear the current line's "Client: " prompt
            std::cout << "\r" << complete_msg << std::endl;
            std::cout << "Client: " << std::flush;

            accumulationBuffer.erase(0, delimiterPos + 1);
        }
    }
}
