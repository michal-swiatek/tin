#include "client/Client.h"

int main() {

    auto * client = new Client();
    client->mynfs_opensession("0.0.0.0", "user1", "superhaslo1");

    return 0;
}