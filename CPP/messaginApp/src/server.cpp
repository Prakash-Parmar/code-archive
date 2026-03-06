#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <atomic>
#include <vector>
#include <mutex>
#include <algorithm>

std::atomic<bool> isRunning{true};
std::vector<int> clientSockets;
std::mutex clientsMutex;

/**
 * Sends a message to all connected clients except the original sender.
 */
void broadcast(const std::string& message, int senderSocket)
{
    std::lock_guard<std::mutex> lock(clientsMutex);
    
    for (int recipientSocket : clientSockets)
    {
        // Don't send the message back to the person who sent it
        if (recipientSocket != senderSocket)
        {
            std::string formattedMsg = "Client " + std::to_string(senderSocket) + ": " + message + "\n";
            send(recipientSocket, formattedMsg.c_str(), formattedMsg.size(), 0);
        }
    }
}

/**
 * Thread function to handle receiving messages from a specific client.
 */
void handleClient(int clientSocket)
{
    char buffer[1024] = {0};
    std::string accumulationBuffer = "";
    size_t delimiterPos = 0;

    while (isRunning)
    {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        // Handle disconnect or error
        if (bytesReceived <= 0)
        {
            if (bytesReceived == 0) {
                std::cout << "[-] Client " << clientSocket << " disconnected." << std::endl;
            } else {
                std::cerr << "[!] Error on socket " << clientSocket << ". errno: " << errno << std::endl;
            }

            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
            }
            
            close(clientSocket);
            break;
        }

        // Add received data to our "backlog" string
        accumulationBuffer.append(buffer, bytesReceived);

        // Extract and broadcast all complete messages (delimited by \n)
        while ((delimiterPos = accumulationBuffer.find('\n')) != std::string::npos)
        {
            std::string completeMessage = accumulationBuffer.substr(0, delimiterPos);
            
            std::cout << "[+] Relay from " << clientSocket << ": " << completeMessage << std::endl;
            broadcast(completeMessage, clientSocket);

            accumulationBuffer.erase(0, delimiterPos + 1);
        }
    }
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Could not create server socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse the address (avoids "Address already in use" errors)
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Bind failed on port 8080. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 10) < 0)
    {
        std::cerr << "Listen failed. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "[*] Server started. Listening on port 8080..." << std::endl;

    while (isRunning)
    {
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        if (clientSocket < 0)
        {
            if (isRunning) {
                std::cerr << "Accept failed. errno: " << errno << std::endl;
            }
            continue;
        }

        std::cout << "[+] New connection from Client " << clientSocket << std::endl;

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clientSockets.push_back(clientSocket);
        }

        // Handle this client in a background thread
        std::thread(handleClient, clientSocket).detach();
    }

    close(serverSocket);
    return 0;
}