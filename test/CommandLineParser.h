//
// Created by micho6 on 24.01.2021.
//

#ifndef TIN_COMMANDLINEPARSER_H
#define TIN_COMMANDLINEPARSER_H

#include <vector>
#include <string>

class CommandLineParser
{
public:
    CommandLineParser(int argc, char** argv);

    bool commandPresent(const std::string& command);
    std::string getParam(const std::string& command, bool required = false);

    static void printHelp();

private:
    bool checkParam(std::vector<std::string>::iterator it);

    std::vector<std::string> tokens;
};


#endif //TIN_COMMANDLINEPARSER_H
