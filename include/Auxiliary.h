#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

class Auxiliary{
    public:
        static std::vector<std::string> parseArguments(const std::string& line);
        static std::vector<std::string> readConfigFile(const std::string &configFilePath);
};
