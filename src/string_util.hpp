// string_util.hpp
// Ashish D'Souza

#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP

#include <string>
#include <vector>
#include <rapidjson/document.h>


namespace PistacheAddons {
    namespace StringUtil {
        std::string trim(const std::string &str);

        std::string toLowercase(const std::string &str);

        std::vector<std::string> split(const std::string &str, const char &delimiter);

        std::string urlencode(const std::string &str);

        std::string urldecode(const std::string &str);
    }
}

#endif
