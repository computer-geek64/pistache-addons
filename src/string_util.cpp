// string_util.cpp
// Ashish D'Souza

#include <string>
#include <vector>
#include <sstream>
#include <rapidjson/document.h>
#include "string_util.hpp"

using namespace std;


string PistacheAddons::StringUtil::trim(const string &str) {
    string str2 = str;
    while (!str2.empty() && (str2.front() == ' ' || str2.front() == '\r' || str2.front() == '\n' || str2.front() == '\t')) str2.erase(0, 1);
    while (!str2.empty() && (str2.back() == ' ' || str2.back() == '\r' || str2.back() == '\n' || str2.back() == '\t')) str2.pop_back();
    return str2;
}

string PistacheAddons::StringUtil::toLowercase(const string &str) {
    string lowercase;
    for (unsigned int i = 0; i < str.length(); i++) lowercase.push_back(tolower(str[i]));
    return lowercase;
}

vector<string> PistacheAddons::StringUtil::split(const string &str, const char &delimiter) {
    vector<string> tokens;
    stringstream stringStream(str);
    string token;
    while (getline(stringStream, token, delimiter)) tokens.push_back(token);
    return tokens;
}

string PistacheAddons::StringUtil::urlencode(const string &str) {
    string str2 = str;
    for (unsigned int i = 0; i < str2.length(); i++) {
        if ((str2[i] < 48 || str2[i] > 57) && (str2[i] < 65 || str2[i] > 90) && (str2[i] < 97 || str2[i] > 122)) {
            stringstream stringStream;
            stringStream << hex << (int) str2[i];
            string encoding(1, '%');
            encoding += stringStream.str().substr(0, 2);

            str2.replace(i, 1, encoding);
            i += 2;
        }
    }

    return str2;
}

string PistacheAddons::StringUtil::urldecode(const std::string &str) {
    string str2 = str;
    int index;
    int offset = 0;
    while ((index = str2.substr(offset, str2.length() - offset).find("%")) != string::npos) {
        stringstream stringStream;
        stringStream << hex << str2.substr((offset += index) + 1, 2);
        int c;
        stringStream >> c;
        str2.replace(offset++, 3, string(1, (char) c));
    }
    return str2;
}
