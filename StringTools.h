#pragma once
#include <string>
namespace xtr{
	inline std::string ArgsStringConcat(){
		return std::string();
	}
	template<typename single_type>
	inline std::string ArgsStringConcat(single_type&& arg){
		return std::to_string(arg);
	}
	// Concatamerize a series of parameters into a unique string parameters may not contain a decimal '.'
	template<typename single_type, typename... multi_types>
	inline std::string ArgsStringConcat(single_type&& single, multi_types&&...args){
		return std::to_string(single) + "." + ArgsStringConcat(args...);
	}
	//Trim values after a decimal to specified significant figures
	std::string trimDecimal(const std::string& input_string, const size_t significant_figures = 2);
}
