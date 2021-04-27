// request_body.hpp
// Ashish D'Souza

#ifndef REQUEST_BODY_HPP
#define REQUEST_BODY_HPP

#include <string>
#include <unordered_map>


std::tuple<std::unordered_map<std::string, std::string>, std::unordered_map<std::string, std::string>> multipart_formData(std::string requestBody, std::string boundary);

#endif
