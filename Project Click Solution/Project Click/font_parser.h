//Includes:
#include <vector>
#include <string>
#include <dirent.h>
#include <map>
#include <iostream>

//Prototypes
std::map< char, std::string > getFontStrings(const char *fontDir);	//Returns a vector with paths to files for each symbol in the font