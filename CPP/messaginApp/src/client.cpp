#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int main() {

    // Creating a socket + error handling
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1){
        std::cout << "Failed to create a socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    // serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Convert IP string → binary form
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);


    // sending connection request + error handling
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cout << "failed to connect. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string send_message;
    char buffer_recv[1024] = {0};

    while(true){
        send_message = "";

        std::cout << "Client: ";
        std::getline(std::cin, send_message);
        send(clientSocket, send_message.c_str(), send_message.size(), 0);

        std::memset(buffer_recv, 0, sizeof(buffer_recv));
        int bytesReceived = recv(clientSocket, buffer_recv, sizeof(buffer_recv), 0); 

        if(bytesReceived == 0){
            std::cout << "Server disconnected" << std::endl;
            break;
        }

        if(bytesReceived < 0){
            std::cout << "recv erro. errno: " << errno << std::endl;
            break;
        }

        std::cout << "Server: ";
        std::cout << buffer_recv << std::endl;
        

    }

   
   

    close(clientSocket);

    return 0;
}