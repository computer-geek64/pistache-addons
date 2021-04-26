// request_body.cpp
// Ashish D'Souza

#include <string>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include "endpoint_util.hpp"
#include "request_body.hpp"

using namespace std;


PostValue::PostValue() : file(false), value("") {}

PostValue::PostValue(bool file, std::string value) : file(file), value(value) {}

void PostValue::setFile(bool file) {
    this->file = file;
}

void PostValue::setValue(std::string value) {
    this->value = value;
}

bool PostValue::getFile() {
    return file;
}

string PostValue::getValue() {
    return value;
}

PostValue::~PostValue() {}

unordered_map<string, PostValue> multipart_formData(string requestBody, string boundary) {
    unordered_map<string, PostValue> variables;

    string line;
    stringstream requestBodyStringStream(requestBody);
    while (getline(requestBodyStringStream, line)) {
        line = trim(line);

        // Boundary
        if (line.compare(boundary) == 0) {
            // Parse Content-Disposition
            getline(requestBodyStringStream, line);
            line = trim(line);
            stringstream contentDispositionStringStream(line.substr(31, line.length() - 31));

            // Extract name
            string name;
            getline(contentDispositionStringStream, name, ';');
            stringstream nameStringStream(trim(name));
            getline(nameStringStream, name, '=');
            getline(nameStringStream, name);
            name = trim(name);
            name = name.substr(1, name.length() - 2);

            // Check if variable or file
            PostValue postValue;
            streampos lastPosition;
            if (!contentDispositionStringStream.eof()) {
                // Extract filename
                postValue.setFile(true);

                string filename;
                getline(contentDispositionStringStream, filename, ';');
                stringstream filenameStringStream(trim(filename));
                getline(filenameStringStream, filename, '=');
                getline(filenameStringStream, filename);
                filename = trim(filename);
                filename = filename.substr(1, filename.length() - 2);
                postValue.setValue(filename);

                while (line.compare("") != 0) {
                    getline(requestBodyStringStream, line);
                    line = trim(line);
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
                // Store variable
                while (line.compare("") != 0) {
                    getline(requestBodyStringStream, line);
                    line = trim(line);
                }

                string nextLine;
                getline(requestBodyStringStream, line);
                lastPosition = requestBodyStringStream.tellg();
                getline(requestBodyStringStream, nextLine);
                while (nextLine.substr(0, boundary.length()).compare(boundary) != 0) {
                    line.push_back('\n');
                    postValue.setValue(postValue.getValue() + line);

                    lastPosition = requestBodyStringStream.tellg();
                    line = nextLine;
                    getline(requestBodyStringStream, nextLine);
                }

                if (line.back() == '\r') line.pop_back();
                postValue.setValue(postValue.getValue() + line);
            }

            requestBodyStringStream.seekg(lastPosition);
            variables[name] = postValue;
        }
        else if (line.compare(boundary + "--") == 0) {
            // End boundary
            break;
        }
    }

    return variables;
}
