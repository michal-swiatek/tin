//
// Created by micho6 on 24.01.2021.
//

#include "CommandLineParser.h"

#include <regex>
#include <iostream>

CommandLineParser::CommandLineParser(int argc, char **argv)
{
    if (argc < 2)
    {
        printHelp();
        exit(0);
    }

    for (int i = 1; i < argc; ++i)
        tokens.emplace_back(std::string(argv[i]));
}

bool CommandLineParser::commandPresent(const std::string &command)
{
    return std::find(tokens.begin(), tokens.end(), command) != tokens.end();
}

std::string CommandLineParser::getParam(const std::string &command, bool required)
{
    auto it = std::find(tokens.begin(), tokens.end(), command);

    if (it != tokens.end() && checkParam(it))
        return *++it;
    else if (!required)
        return "";
    else
    {
        printHelp();
        exit(0);
    }
}

bool CommandLineParser::checkParam(std::vector<std::string>::iterator it)
{
    if (std::next(it) == tokens.end())
        return false;

    auto command = *it;
    auto param = *++it;

    if (command == "-ip")
        return std::regex_match(param, std::regex(R"((\b(1?[0-9]{1,2}|2[0-4][0-9]|25[0-5])\b\.){3}\b(1?[0-9]{1,2}|2[0-4][0-9]|25[0-5])\b)"));

    return true;
}

void CommandLineParser::printHelp()
{
    std::cout << "Program usage:\n";
    std::cout << "client -ip ipv4_address [commands...]\n\n";

    std::cout << "Commands can appear in arbitrary order, params should be entered after whitespace.\n";
    std::cout << "List of available commands:\n\n";

    std::cout << "-ip\tREQUIRED, ipv4 address of server to connect\n";
    std::cout << "-session\tOPTIONAL, creates an interactive session\n";
    std::cout << "-v\tOPTIONAL, in non-interactive mode outputs commands\n\n";

    std::cout << "Following commands are required only when using non-interactive session:\n";
    std::cout << "-login\tREQUIRED, login used to connect to server, user will be prompted for password\n";
    std::cout << "-commands\tREQUIRED, commands to be executed in non-interactive session, separated by coma\n";
}
