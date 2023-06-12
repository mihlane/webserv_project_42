# ifndef STRING_HPP
# define STRING_HPP
# include <string>
# include <vector>
#include <iostream>
#include <sys/socket.h>
#include <fstream> // remove


#define buf 1024

std::string trim(const std::string& str, const std::string t);
std::string tok(std::string &str, std::string sep);
bool containsOnlyWhitespaces(const std::string& str);
std::vector<std::string> splitString(const std::string& str, char delimiter);
std::string trimString(const std::string &str);
std::vector<std::string> getlines(int phase);
std::string getRemainder();

#endif