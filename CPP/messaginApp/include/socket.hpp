#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

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
    explicit Socket(int s);
    ~Socket();

    // Prevent copying to ensure unique ownership of the file descriptor
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    /**
     * Receives data and returns a structured result.
     */
    IoResult receive(char* buffer, size_t size);

    /**
     * Sends a string message over the socket.
     */
    void send(const std::string& message);

    /**
     * Unblocks any pending I/O and prevents further reads/writes.
     */
    void shutdownReadWrite();

    /**
     * Closes the socket if it is still open.
     */
    void close();

    /**
     * Translates a binary address into a human-readable identity (IP:Port).
     */
    void setIdentity(const sockaddr_in& addr);

    /**
     * Returns the cached identity string.
     */
    std::string getIdentity() const;
};
