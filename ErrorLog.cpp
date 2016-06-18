#include "stdafx.h"
#include "ErrorLog.h"
using namespace xtr;

ErrorLog::ErrorLog(){}

ErrorLog::~ErrorLog(){}

// NOTE THIS IS NOT THREAD SAFE YOU MUST MAKE IT SO IF USING MULTITHREADING when initializing.  (i.e. initialize before doing any multithreading...)
ErrorLog &ErrorLog::getInstance() {
	static ErrorLog instance; // Guaranteed to be destroyed.
	// Instantiated on first use.
	return instance;
}

// write a message to log
void ErrorLog::writeToLog(std::string text){
	auto&& d = data.Lock();
	if (!d->file_created)
		createFile(*d);
	// Assemble output
	std::string ss = std::to_string(d->log_line_number++) + " " + text + '\n';
	// Write output to file
	d->log_file = fopen(d->log_file_name.c_str(), "a");
	fwrite(ss.c_str(), 1, ss.length(), d->log_file);
	fclose(d->log_file);
}

void ErrorLog::setFileName(std::string name)
{
	auto&& d = data.Lock();
	d->log_file_name = name;
	createFile(*d);
}
// Create a new log file
void ErrorLog::createFile(Data& d)
{
	d.log_file = fopen(d.log_file_name.c_str(), "w");
	fclose(d.log_file);
	d.file_created = true;
	d.log_line_number = 0;
}