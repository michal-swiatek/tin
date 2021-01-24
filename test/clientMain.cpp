#include <iostream>

#include "../client/Client.h"

#include "CommandLineParser.h"
#include "Session.h"

int main(int argc, char** argv) {

    CommandLineParser parser(argc, argv);

    auto host = parser.getParam("-ip", true);
    auto client = new Client();
    auto session = Session(client);

    if (parser.commandPresent("-session"))
    {
        std::string connect, login, password;

        do
        {
            if (client->mynfs_error != NO_ERROR)
                std::cout << mynfs_strerror(client->mynfs_error) << '\n';

            std::cout << "Connect? (y/n):"; std::cin >> connect;

            if (connect == "n")
                exit(0);
            else if (connect != "y")
                continue;

            std::cout << "Enter login:";    std::cin >> login;
            std::cout << "Enter password:"; std::cin >> password;

        } while (client->mynfs_opensession(host.c_str(), login.c_str(), password.c_str()) == -1);

        session.start();
    }
    else
    {
        auto login = parser.getParam("-login", true);
        std::string password;

        std::cout << "Enter password:"; std::cin >> password;

        if (client->mynfs_opensession(host.c_str(), login.c_str(), password.c_str()) == -1)
        {
            std::cout << mynfs_strerror(client->mynfs_error) << '\n';
            exit(0);
        }

        auto tokens = Session::parseLine(parser.getParam("-command"));
        session.executeCommand(tokens);
    }

    client->mynfs_closesession();

    return 0;
}
