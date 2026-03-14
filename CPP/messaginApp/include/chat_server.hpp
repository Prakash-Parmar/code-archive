#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <string>
#include "socket.hpp"

/**
 * Manages the high-level logic for a multi-user Chat Server.
 * Encapsulates client connections, broadcasting, and life-cycle.
 */
class ChatServer {
    int serverSocket;
    std::atomic<bool> isRunning;
    std::vector<std::shared_ptr<Socket>> clientSockets;
    std::mutex clientsMutex;

public:
    ChatServer();
    ~ChatServer();

    // Prevent copying the server instance
    ChatServer(const ChatServer&) = delete;
    ChatServer& operator=(const ChatServer&) = delete;

    /**
     * Starts the server on the specified IP and port and begins accepting connections.
     * This method contains the main accept loop.
     */
    void start(const std::string& ip, int port);


    /**
     * Gracefully stops the server and closes all active connections.
     */
    void stop();

private:
    /**
     * Internal worker function to handle communication with a single client.
     * Runs in its own thread for each connection.
     */
    void handleClient(std::shared_ptr<Socket> client);

    /**
     * Relays a message to all active clients except the sender.
     */
    void broadcast(const std::string& message, const std::shared_ptr<Socket>& sender);

    /**
     * Removes a client from the active list safely.
     */
    void removeClient(const std::shared_ptr<Socket>& client);
};
