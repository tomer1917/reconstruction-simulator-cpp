#include <fstream>
#include <sstream>
#include "Auxiliary.h"
/*
This is a 'static' method that receives a string(line) and returns a vector of the string's arguments.

For example:
parseArguments("settlement KfarSPL 0") will return vector with ["settlement", "KfarSPL", "0"]

To execute this method, use Auxiliary::parseArguments(line)
*/
std::vector<std::string> Auxiliary::parseArguments(const std::string& line) {
    std::vector<std::string> arguments;
    std::istringstream stream(line);
    std::string argument;

    while (stream >> argument) {
        arguments.push_back(argument);
    }

    return arguments;
}

std::vector<std::string> Auxiliary::readConfigFile(const std::string &configFilePath) {
    std::vector<std::string> lines;
    std::ifstream configFile(configFilePath);
    
    // if (!configFile.is_open()) {
    //     throw std::runtime_error("Failed to open configuration file: " + configFilePath);
    // }

    std::string line;
    while (std::getline(configFile, line)) {
        // Skip empty lines or lines starting with a comment
        if (!line.empty() && line[0] != '#') {
            lines.push_back(line);  // Add line to vector
        }
    }

    configFile.close();
    return lines;
}

