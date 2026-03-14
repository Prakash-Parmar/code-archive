#include "chat_server.hpp"

int main() {
    ChatServer server;
    // Bind to all interfaces (0.0.0.0) on port 8080
    server.start("0.0.0.0", 8080);
    return 0;
}
