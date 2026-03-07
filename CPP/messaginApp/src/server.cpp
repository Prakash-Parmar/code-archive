#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <atomic>
#include <vector>
#include <mutex>
#include <algorithm>

/**
 * Represents the outcome of a socket operation.
 */
enum class SocketStatus {
    SUCCESS,
    DISCONNECTED,
    ERR
};

/**
 * Bundles the status and the number of bytes transferred.
 */
struct IoResult {
    SocketStatus status;
    ssize_t bytes;
};

/**
 * RAII Wrapper for a TCP Socket.
 */
class Socket {
    int fd;
    std::string ipAddress;
    int port;

public:
    explicit Socket(int s) : fd(s), port(0) {}

    ~Socket() {
        if (fd >= 0) {
            close(fd);
            std::cout << "[*] Resource Cleaned: Socket " << fd << " closed." << std::endl;
        }
    }

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    /**
     * Receives data and returns a structured result.
     */
    IoResult receive(char* buffer, size_t size) {
        ssize_t n = ::recv(fd, buffer, size, 0);
        if (n > 0) return {SocketStatus::SUCCESS, n};
        if (n == 0) return {SocketStatus::DISCONNECTED, 0};
        return {SocketStatus::ERR, n};
    }

    void send(const std::string& message) {
        ::send(fd, message.c_str(), message.size(), 0);
    }

    void setIdentity(const sockaddr_in& addr) {
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, ipStr, INET_ADDRSTRLEN);
        ipAddress = ipStr;
        port = ntohs(addr.sin_port);
    }

    std::string getIdentity() const {
        return ipAddress + ":" + std::to_string(port);
    }
};

// Global State
std::atomic<bool> isRunning{true};
std::vector<std::shared_ptr<Socket>> clientSockets;
std::mutex clientsMutex;

/**
 * Relays a message to all connected clients except the sender.
 */
void broadcast(const std::string& message, const std::shared_ptr<Socket>& sender) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    std::string formattedMsg = sender->getIdentity() + ": " + message + "\n";

    for (const auto& recipient : clientSockets) {
        if (recipient != sender) {
            recipient->send(formattedMsg);
        }
    }
}

/**
 * Manages the lifecycle of a single client connection.
 */
void handleClient(std::shared_ptr<Socket> client) {
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

            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                clientSockets.erase(
                    std::remove(clientSockets.begin(), clientSockets.end(), client), 
                    clientSockets.end()
                );
            }
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

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Fatal: Could not create server socket." << std::endl;
        return EXIT_FAILURE;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Fatal: Bind failed on port 8080." << std::endl;
        return EXIT_FAILURE;
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Fatal: Listen failed." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "[*] Server active on port 8080. Awaiting connections..." << std::endl;

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

        std::thread(handleClient, client).detach();
    }

    close(serverSocket);
    return 0;
}