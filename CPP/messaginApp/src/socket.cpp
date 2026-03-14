#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "socket.hpp"

Socket::Socket(int s) : fd(s), port(0) {}

Socket::~Socket() {
    close();
}

IoResult Socket::receive(char* buffer, size_t size) {
    ssize_t n = ::recv(fd, buffer, size, 0);

    if (n > 0) {
    std::cout.write(buffer, n);
    }
    
    if (n > 0) return {SocketStatus::SUCCESS, n};
    if (n == 0) return {SocketStatus::DISCONNECTED, 0};
    return {SocketStatus::ERR, n};
}

void Socket::send(const std::string& message) {
    ::send(fd, message.c_str(), message.size(), 0);
}

void Socket::shutdownReadWrite() {
    if (fd >= 0) {
        ::shutdown(fd, SHUT_RDWR);
    }
}

void Socket::close() {
    if (fd >= 0) {
        ::close(fd);
        std::cout << "[*] RAII Cleanup: Socket " << fd << " closed." << std::endl;
        fd = -1;
    }
}

void Socket::setIdentity(const sockaddr_in& addr) {
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ipStr, INET_ADDRSTRLEN);
    ipAddress = ipStr;
    port = ntohs(addr.sin_port);
}

std::string Socket::getIdentity() const {
    return ipAddress + ":" + std::to_string(port);
}
