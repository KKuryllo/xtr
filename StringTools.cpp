#include "stdafx.h"
#include "StringTools.h"
namespace xtr{
    std::string trimDecimal(const std::string& input_string, const size_t significant_figures){
        std::string working_string = input_string;
        size_t position = 0;
        for (; position < working_string.size(); ++position) {
            if (working_string[position] == '.') {  // Find position of decimal (if it exists)
                break;
            }
        }
        if (position == working_string.size()) working_string += '.' + std::string(significant_figures, '0');  // if no decimal add one and significant figures
        else working_string.resize(position + 1 + significant_figures, '0'); // else change number of significant figures
        return working_string;
    }
}
