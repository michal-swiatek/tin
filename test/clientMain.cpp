#include <iostream>

#include "../client/NFSClient.h"

#include "CommandLineParser.h"
#include "Session.h"

int main(int argc, char **argv) {

    CommandLineParser parser(argc, argv);

    auto host = parser.getParam("-ip", true);
    auto client = new NFSClient();
    auto session = Session(client);

    if (parser.commandPresent("-session")) {
        std::string connect, login, password;

        do {
            if (client->mynfs_error != NO_ERROR)
                std::cout << mynfs_strerror(client->mynfs_error) << '\n';

            std::cout << "Connect? (y/n):";
            std::cin >> connect;

            if (connect == "n")
                exit(0);
            else if (connect != "y")
                continue;

            std::cout << "Enter login:";
            std::cin >> login;
            std::cout << "Enter password:";
            std::cin >> password;

        } while (client->mynfs_opensession(host.c_str(), login.c_str(), password.c_str()) == -1);

        session.start();
    } else {
        auto login = parser.getParam("-login", true);
        auto commands = parser.getParam("-commands", true);
        auto verbose = parser.commandPresent("-v");
        std::string password;

        std::cout << "Enter password:";
        std::cin >> password;

        if (client->mynfs_opensession(host.c_str(), login.c_str(), password.c_str()) == -1) {
            std::cout << mynfs_strerror(client->mynfs_error) << '\n';
            exit(0);
        }

        size_t pos = 0;
        while ((pos = commands.find(',')) != std::string::npos) {
            auto command = commands.substr(0, pos);

            if (verbose)
                std::cout << ">> " << command << '\n';

            auto tokens = session.parseLine(command);
            session.executeCommand(tokens);

            commands.erase(0, pos + 1);
        }

        //  Execute last command
        auto command = commands.substr(0, pos);

        if (verbose)
            std::cout << ">> " << command << '\n';

        auto tokens = session.parseLine(command);
        session.executeCommand(tokens);

        commands.erase(0, pos + 1);
    }

    client->mynfs_closesession();
    delete client;

    return 0;
}
