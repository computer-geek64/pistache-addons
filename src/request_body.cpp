// request_body.cpp
// Ashish D'Souza

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include "string_util.hpp"
#include "request_body.hpp"

using namespace std;


int PistacheAddons::RequestBody::parseMultipartFormData(const string &requestBody, const string &boundary, unordered_map<string, string> &variables, unordered_map<string, string> &files) {
    string line;
    stringstream requestBodyStringStream(requestBody);
    while (getline(requestBodyStringStream, line)) {
        line = StringUtil::trim(line);

        // Boundary
        if (line.compare(boundary) == 0) {
            // Parse Content-Disposition
            getline(requestBodyStringStream, line);
            line = StringUtil::trim(line);
            stringstream contentDispositionStringStream(line.substr(31, line.length() - 31));

            // Extract name
            string name;
            getline(contentDispositionStringStream, name, ';');
            name = StringUtil::trim(StringUtil::split(StringUtil::trim(name), '=')[1]);
            name = name.substr(1, name.length() - 2);
            cout << "Testing... " << name;
            int index;
            int offset = 0;
            while ((index = name.substr(offset, name.length() - offset).find("\\\"")) != string::npos) name.replace((offset += index)++, 2, "\"");

            // Check if variable or file
            streampos lastPosition;
            if (!contentDispositionStringStream.eof()) {
                // Extract filename
                string filename;
                getline(contentDispositionStringStream, filename, ';');
                filename = StringUtil::trim(StringUtil::split(StringUtil::trim(filename), '=')[1]);
                filename = filename.substr(1, filename.length() - 2);
                int index;
                int offset = 0;
                while ((index = filename.substr(offset, filename.length() - offset).find("\\\"")) != string::npos) filename.replace((offset += index)++, 2, "\"");
                files[name] = filename;

                while (line.compare("") != 0) {
                    getline(requestBodyStringStream, line);
                    line = StringUtil::trim(line);
                }

                ofstream file(filename);
                string nextLine;
                getline(requestBodyStringStream, line);
                lastPosition = requestBodyStringStream.tellg();
                getline(requestBodyStringStream, nextLine);
                while (nextLine.substr(0, boundary.length()).compare(boundary) != 0) {
                    line.push_back('\n');
                    file.write(line.c_str(), line.length());

                    lastPosition = requestBodyStringStream.tellg();
                    requestBodyStringStream.tellg();
                    line = nextLine;
                    getline(requestBodyStringStream, nextLine);
                }

                if (line.back() == '\r') line.pop_back();
                file.write(line.c_str(), line.length());
                file.close();
            }
            else {
                // Extract variable
                while (line.compare("") != 0) {
                    getline(requestBodyStringStream, line);
                    line = StringUtil::trim(line);
                }

                string variable;
                string nextLine;
                getline(requestBodyStringStream, line);
                lastPosition = requestBodyStringStream.tellg();
                getline(requestBodyStringStream, nextLine);
                while (nextLine.substr(0, boundary.length()).compare(boundary) != 0) {
                    line.push_back('\n');
                    variable += line;

                    lastPosition = requestBodyStringStream.tellg();
                    line = nextLine;
                    getline(requestBodyStringStream, nextLine);
                }

                if (line.back() == '\r') line.pop_back();
                variable += line;
                variables[name] = variable;
            }

            requestBodyStringStream.seekg(lastPosition);
        }
        else if (line.compare(boundary + "--") == 0) {
            // End boundary
            break;
        }
    }

    return 0;
}

int PistacheAddons::RequestBody::parseApplicationFormUrlencoded(const string &requestBody, unordered_map <string, string> &variables) {
    vector<string> params = StringUtil::split(requestBody, '&');
    for (unsigned int i = 0; i < params.size(); i++) {
        vector<string> variable = StringUtil::split(params[i], '=');
        variables[StringUtil::urldecode(variable[0])] = StringUtil::urldecode(variable[1]);
    }

    return 0;
}
