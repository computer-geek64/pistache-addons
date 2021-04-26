// endpoint_util.hpp
// Ashish D'Souza

#ifndef ENDPOINT_UTIL_HPP
#define ENDPOINT_UTIL_HPP

#include <string>

#define CORS(responseWriter) responseWriter.headers().add<Http::Header::AccessControlAllowOrigin>("*")


std::string trim(const std::string &str) {
    std::string str2 = str;
    while (!str2.empty() && (str2.front() == ' ' || str2.front() == '\r' || str2.front() == '\n' || str2.front() == '\t')) str2.erase(0, 1);
    while (!str2.empty() && (str2.back() == ' ' || str2.back() == '\r' || str2.back() == '\n' || str2.back() == '\t')) str2.pop_back();
    return str2;
}

std::string toLowercase(const std::string &str) {
    std::string lowercase;
    for (unsigned int i = 0; i < str.length(); i++) lowercase.push_back(std::tolower(str[i]));
    return lowercase;
}

#endif
