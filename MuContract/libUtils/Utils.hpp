#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <cstdlib>

namespace SolInstru{
namespace Utils{
    void debug_info(std::string _info);

    // trim string from left side
    void ltrim(std::string& _str);

    // trim string from right side
    void rtrim(std::string& _str);

    // trim from both ends (in place)
    void trim(std::string& _str);

    // trim from left side but no overwrite
    std::string ltrim_copy(std::string& _str);

    // trim from right side but no overwrite
    std::string rtrim_copy(std::string& _str);

    // trim from both sides but no overwrite
    std::string trim_copy(std::string& _str);

    std::vector<std::string> split(std::string& _str, std::string _delimiter);

    // Get the _index[th] element from a string split by _c
    std::string retrieve_string_element(std::string& _str, unsigned int _index, std::string _delimiter);

    std::string substr_by_edge(std::string _str, std::string _left, std::string _right);

    //from https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
    void str_replace_all(std::string& _str, const std::string& _from, const std::string& _to);
}
}