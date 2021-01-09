//
// Created by ula on 07.01.2021.
//

#include "client.h"

int main() {

    Client * client = new Client();
    client->mynfs_opensession("0.0.0.0", "user1", "superhaslo1");

    return 0;
}
