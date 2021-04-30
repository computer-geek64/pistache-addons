// request_body.hpp
// Ashish D'Souza

#ifndef REQUEST_BODY_HPP
#define REQUEST_BODY_HPP

#include <string>
#include <unordered_map>


namespace PistacheAddons {
    namespace RequestBody {
        int parseMultipartFormData(const std::string &requestBody, const std::string &boundary, std::unordered_map <std::string, std::string> &variables, std::unordered_map <std::string, std::string> &files);

        int parseApplicationFormUrlencoded(const std::string &requestBody, std::unordered_map<std::string, std::string> &variables);
    }
}

#endif
