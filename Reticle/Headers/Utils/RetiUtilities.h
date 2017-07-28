#ifndef RETIUTILITIES_H_INCLUDED
#define RETIUTILITIES_H_INCLUDED

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace RetiUtilities
{
    void LoadFileToString(std::string& data, std::ifstream& file);
    std::string SubstrByChar(char bch, char ech, size_t& start, const std::string& str);
    void TokenizeString(const std::string& data, char ch, std::vector<std::string>& out);
    std::string TrimString(const std::string& str, const std::string& charset);
}

#endif // RETIUTILITIES_H_INCLUDED
