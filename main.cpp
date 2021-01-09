#include "server/Server.h"
#include "client/Client.h"

int main() {

    Server server(5);

    server.setup();
    server.run();
    server.close();

    //Client client;

    return 0;
}
