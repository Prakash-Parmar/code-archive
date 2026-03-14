#include "chat_client.hpp"

int main() {
    ChatClient client;
    if (client.connect("127.0.0.1", 8080)) {
        client.run();
    }
    return 0;
}
