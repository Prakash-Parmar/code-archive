#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>



int main() {

    // Creating a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // error handling
    if(serverSocket == -1){
        std::cout << "Failed to create a socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket + error handling
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cout << "failed to bind to port 8080. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // listening to the assigned socket (hold at most 5 connections in queue) + error handling
    if (listen(serverSocket, 5) < 0)
    {
        std::cout << "failed to listen in the socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // accepting connection request from the queue
    auto addrlen = sizeof(serverAddress);
    int connection = accept(serverSocket, nullptr, nullptr);

    // error handling
    if (connection < 0)
    {
        std::cout << "failed to grab a connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    char buffer_recv[1024] = {0};
    std::string send_message;

    while(true){
        std::memset(buffer_recv, 0, sizeof(buffer_recv));
        
        int bytesReceived = recv(connection, buffer_recv, sizeof(buffer_recv), 0);

        if(bytesReceived == 0){
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        if(bytesReceived < 0){
            std::cout << "recv erro. errno: " << errno << std::endl;
            break;
        }

        std::cout << "Client: " << buffer_recv << std::endl;

        send_message = "";
        std::cout << "Server: ";
        std::getline(std::cin, send_message);
        
        send(connection, send_message.c_str(), send_message.size(), 0);

    
    }
    
 
    

    // closing the socket and connection
    close(connection);
    close(serverSocket);

    return 0;
}