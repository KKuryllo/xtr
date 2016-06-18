#include <string>
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"

namespace xtr{
namespace ReadFile{
std::string readAsString(const std::string& file_name);
template<typename Handler>
rapidjson::ParseResult readJSON(const std::string& file_name, Handler& handler)
{
	auto&& data = readAsString(file_name);
	const char* chars = data.c_str();
	rapidjson::StringStream json_stream(chars);
	rapidjson::Reader reader;
	rapidjson::ParseResult result = reader.Parse(json_stream, handler);
	if (!result) LOG("Error: Parsing " + file_name + ". " + std::string(GetParseError_En(result.Code())) + " At: " + std::to_string(result.Offset()));
	return result;
}
}
}
