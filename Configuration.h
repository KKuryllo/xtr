#pragma once
namespace xtr{
	class Configuration {
	public:
		Configuration(const std::string& desired_name);
		virtual ~Configuration();	
		void saveFile();
		void loadFile();
		int getConfigAsInt(std::string tag_name, int default_value);
		std::string getConfigAsString(std::string tag_name, std::string default_value);
		bool getConfigAsBool(std::string tag_name, bool default_value);
		int setConfigAsInt(std::string tag_name, int new_value);
		std::string setConfigAsString(std::string tag_name, std::string new_value);
		bool setConfigAsBool(std::string tag_name, bool new_value);
		std::map<std::string, std::string> data;
	private:
		std::string configuration_file;
	};
}

