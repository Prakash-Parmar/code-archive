#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <memory>

#include "socket.hpp"

// Global flag to manage the application lifecycle
std::atomic<bool> isRunning{true};

/**
 * Background thread function to handle incoming messages from the server.
 */
void recvMessage(std::shared_ptr<Socket> clientSocket) {
    char buffer_recv[1024];
    std::string accumulationBuffer = "";

    while (isRunning) {
        IoResult result = clientSocket->receive(buffer_recv, sizeof(buffer_recv));

        if (result.status != SocketStatus::SUCCESS) {
            if (result.status == SocketStatus::DISCONNECTED) {
                std::cout << "\n[!] Connection lost: Server closed the connection." << std::endl;
            } else {
                std::cerr << "\n[!] Error receiving from server. errno: " << errno << std::endl;
            }
            isRunning = false; // Stop the main loop
            break;
        }

        accumulationBuffer.append(buffer_recv, result.bytes);

        // Extract and print all complete line-delimited messages
        size_t delimiterPos;
        while ((delimiterPos = accumulationBuffer.find('\n')) != std::string::npos) {
            std::string complete_msg = accumulationBuffer.substr(0, delimiterPos);
            
            // Move cursor to start of line to clear "Client: " prompt, then print server msg
            std::cout << "\r" << complete_msg << std::endl;
            std::cout << "Client: " << std::flush;

            accumulationBuffer.erase(0, delimiterPos + 1);
        }
    }
}

int main() {
    // 1. Create raw socket
    int rawSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (rawSocket == -1) {
        std::cerr << "Failed to create a socket. errno: " << errno << std::endl;
        return EXIT_FAILURE;
    }

    // 2. Wrap in RAII Socket class for automatic cleanup
    auto clientSocket = std::make_shared<Socket>(rawSocket);

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // 3. Connect to the server
    if (connect(rawSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to server at 127.0.0.1:8080. errno: " << errno << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "[*] Connected to server. Type your message and press Enter." << std::endl;

    // 4. Start the background receive thread
    std::thread recvThread(recvMessage, clientSocket);

    // 5. Main Loop: Send messages from standard input
    std::string send_message;
    while (isRunning) {
        std::cout << "Client: " << std::flush;
        if (!std::getline(std::cin, send_message)) break;

        if (!send_message.empty()) {
            clientSocket->send(send_message + "\n");
        }
    }

    isRunning = false;
    if (recvThread.joinable()) {
        recvThread.join();
    }

    return 0;
}
