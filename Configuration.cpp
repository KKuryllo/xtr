#include "stdafx.h"
#include "Configuration.h"
#include "ErrorLog.h"
using namespace xtr;

Configuration::Configuration(const std::string& desired_name) : configuration_file(desired_name){}

// Commands destructor
Configuration::~Configuration(){}

// Retrieves a the value of a configuration tag as an int
// If the value does not exist the default_value will be returned
int Configuration::getConfigAsInt(std::string tag_name, int default_value){
	// find value if it does not exist an extry will be created with an empty string
	std::string value = data[tag_name];
	// If entry is empty set it to the default and return the default
	if (value.empty()){
		data[tag_name] = std::to_string(default_value);
		return default_value;
	}
	// if it's not empty attempt to convert it to an int and return the value
	else{
		try{
			return std::stoi(value);
		}
		// if the fails log the error and return the default value
		catch (const std::invalid_argument& e){
			LOG("Error: Could not covert value: " + std::string(e.what()) + " to an int, for tag: " + tag_name + "/n. Possible duplication of tag name");
			return default_value;
		}
	}
}

// Retrieves a the value of a configuration tag as an int
// If the value does not exist the default_value will be returned
std::string Configuration::getConfigAsString(std::string tag_name, std::string default_value){
	std::string value = data[tag_name];
	// If entry is empty set it to the default and return the default
	if (value.empty()){
		data[tag_name] = default_value;
		return default_value;
	}
	// if it's not empty return value
	else return value;
}

// Retrieves a the value of a configuration tag as an int
// If the value does not exist the default_value will be returned
bool Configuration::getConfigAsBool(std::string tag_name, bool default_value){
	// find value if it does not exist an extry will be created with an empty string
	std::string value = data[tag_name];
	// If entry is empty set it to the default and return the default
	if (value.empty()){
		data[tag_name] = default_value ? "1" : "0";
		return default_value;
	}
	// if it's not empty attempt to convert it to a string and return the value
	else{
		try{
			if (value == "0") return false;
			else if (value == "1") return true;
			else throw (value);
		}
		// if the fails log the error and return the default value
		catch (const std::string& e){
			LOG("Error: Could not covert value: " + e + " to an bool, for tag: " + tag_name + "/n. Possible duplication of tag name");
			return default_value;
		}
	}
}

// Sets configuration tag with the specified value and returns value.  If tag does not exist it will create it.
int Configuration::setConfigAsInt(std::string tag_name, int new_value){
	data[tag_name] = std::to_string(new_value);
	return new_value;
}

// Sets configuration tag with the specified value and returns value.  If tag does not exist it will create it.
std::string Configuration::setConfigAsString(std::string tag_name, std::string new_value){
	data[tag_name] = new_value;
	return new_value;
}

// Sets configuration tag with the specified value and returns value.  If tag does not exist it will create it.
bool Configuration::setConfigAsBool(std::string tag_name, bool new_value){
	data[tag_name] = std::to_string(new_value);
	return new_value;
}

// read configuration from config file
void Configuration::loadFile() {
	// checks if a character is a standard (non-special) character
	auto&& isStandardChar = [](const char c) {
		if (c >= 33 && c <= 126) return true;
		else return false;
	};

	std::string line;
	std::ifstream the_file(configuration_file);
	// what to do if the file exists
	if (the_file.is_open()) {
		std::string tag, value;
		bool has_tag;
		// loop through every line in the file
		while (std::getline(the_file, line)) {
			has_tag = false;
			// loop through every character in line and assign either to tag or value
			for (char& c : line) {
				switch (c) {
				case '=':
					if (tag.length() > 0) has_tag = true;
					break;
				default:
					if (isStandardChar(c)) {
						if (has_tag) value += c;
						else tag += c;
					}
					break;
				}
			}
			// Check if line was properly formed
			if (has_tag && value.length() > 0)
				data[tag] = value;
			tag.clear();
			value.clear();
		}
		the_file.close();
	}
}

// write configuration from config file
void Configuration::saveFile() {
	std::ofstream the_file(configuration_file);
	if (the_file.is_open()) {
		for (auto&& iter : data)
			the_file << iter.first << "=" << iter.second << std::endl;
		the_file.close();
	}
}