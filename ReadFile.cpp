#include "stdafx.h"
#include "ReadFile.h"
#include "ErrorLog.h"
using namespace xtr;
std::string ReadFile::readAsString(const std::string& file_name){
	std::string line;
	std::stringstream ss;

	std::ifstream the_file(file_name);
	if (the_file.is_open()){
		std::getline(the_file, line);
		if (line.substr(0, 3) == std::string({ char(0xEF), char(0xBB), char(0xBF) }))	// Remove dumb shit utf-8 BOM if present
			ss << line.substr(3, line.size() - 3);
		else ss << line;
		while (std::getline(the_file, line))
			ss << line;
	the_file.close();
	}
	else LOG("Error: When trying to read file " + file_name + "\"");
	return ss.str();
}

