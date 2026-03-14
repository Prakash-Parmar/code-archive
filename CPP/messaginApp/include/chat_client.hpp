#pragma once

#include <string>
#include <memory>
#include <atomic>
#include <thread>
#include "socket.hpp"

/**
 * Manages the high-level logic for a Chat Client.
 * Encapsulates the connection, the receive thread, and UI interaction.
 */
class ChatClient {
    std::shared_ptr<Socket> clientSocket;
    std::atomic<bool> isRunning;
    std::thread recvThread;

public:
    ChatClient();
    ~ChatClient();

    // Prevent copying the client instance
    ChatClient(const ChatClient&) = delete;
    ChatClient& operator=(const ChatClient&) = delete;

    /**
     * Connects to a server at the specified IP and port.
     */
    bool connect(const std::string& ip, int port);

    /**
     * Starts the main interaction loop (reading from stdin and sending).
     */
    void run();

    /**
     * Gracefully disconnects from the server.
     */
    void stop();

private:
    /**
     * Background worker function to handle incoming messages from the server.
     */
    void receiveLoop();
};
