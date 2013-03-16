#include "font_parser.h"

std::map< char, std::string > getFontStrings(const char *fontDir)
{
	std::map< char, std::string > fontStrings;				//Map that will be returned

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(fontDir)) != NULL)				//Try to open the directory given
	{
		while ((ent = readdir(dir)) != NULL)			//Add the paths to all files and directories to fontStrings
		{
			std::string fileName = std::string(ent->d_name);
			std::string key1 = fileName.substr(0, fileName.length()-4);
			char key = (char) atoi(key1.c_str());
			std::cout << key << " " << fontDir << fileName << std::endl;
			fontStrings[key] = (fontDir + fileName);
			//std::cout << ent->d_name << std::endl;
		}
		closedir(dir);
	}
	else 
	{
		std::cout << "could not open directory" << std::endl;
		//return some error exception thingything
	}

//	fontStrings["."].erase();
//	fontStrings[".."].erase();
	return fontStrings;
}


char a3toc(const char *ptr)
{
    return (ptr[0]-'0')*100 + (ptr[1]-'0')*10 + (ptr[0]-'0');
}
