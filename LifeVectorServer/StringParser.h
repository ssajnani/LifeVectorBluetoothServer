/**
 * @file StringParser.h
 * @author Rukun Wang (rwang264@uwo.ca)
 * @brief Class used to help parse and split strings
 * @version 0.1
 */
#ifndef STRING_PARSER_H
#define STRING_PARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

/**
 * @brief Class used to help parse and split strings
 * 
 */
class StringParser
{
  public:
  /**
   * @brief Parses a string by a delimiter and stores split string
   * into the vector
   * 
   * @param str string - input string
   * @param container vector<string> - the output container
   * @param delim char - the character to split the string by 
   */
    static void custom_parse(const std::string &str, std::vector<std::string> &container, char delim)
    {
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delim))
        {
            container.push_back(token);
        }
    }
};

#endif