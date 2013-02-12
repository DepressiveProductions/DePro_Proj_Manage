#include "font_parser.h"

std::map< std::string, std::string > getFontStrings(const char *fontDir)
{
	std::map< std::string, std::string > fontStrings;				//Map that will be returned

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(fontDir)) != NULL)				//Try to open the directory given
	{
		while ((ent = readdir(dir)) != NULL)			//Add the paths to all files and directories to fontStrings
		{
			std::string fileName = std::string(ent->d_name);
			std::string key = "FONT_" + fileName.substr(0, fileName.length()-4);
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

	fontStrings["FONT_."].erase();
	fontStrings["FONT_.."].erase();
	return fontStrings;
}


