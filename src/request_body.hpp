// request_body.hpp
// Ashish D'Souza

#ifndef REQUEST_BODY_HPP
#define REQUEST_BODY_HPP

#include <string>
#include <unordered_map>


class PostValue {
    bool file;
    std::string value;

public:
    PostValue();

    PostValue(bool file, std::string value);

    void setFile(bool file);

    void setValue(std::string value);

    bool getFile();

    std::string getValue();

    ~PostValue();
};

std::unordered_map<std::string, PostValue> multipart_formData(std::string requestBody, std::string boundary);

#endif
