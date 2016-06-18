#pragma once
#include "MultithreadingDefines.h"
#include "MutexedObject.h"
#include <cstdint>

#define LOG xtr::ErrorLog::getInstance().writeToLog

namespace xtr{
	class ErrorLog{
	private:
		struct Data {
			FILE* log_file;
			std::string log_file_name = "error.log";
			std::uint64_t log_line_number = 0;
			bool file_created = false;
		};
	public:
		static ErrorLog& getInstance();
		void writeToLog(std::string text);
		void setFileName(std::string name);
	private:
		ErrorLog();
		virtual ~ErrorLog();
		ErrorLog(ErrorLog const&) = delete; // Don't Implement
		void operator=(ErrorLog const&) = delete; // Don't implement
		void createFile(Data& d);
		MutexedObject<Data> data;
	};
}