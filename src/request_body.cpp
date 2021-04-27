// request_body.cpp
// Ashish D'Souza

#include <string>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include "endpoint_util.hpp"
#include "request_body.hpp"

using namespace std;


tuple<unordered_map<string, string>, unordered_map<string, string>> multipart_formData(string requestBody, string boundary) {
    tuple<unordered_map<string, string>, unordered_map<string, string>> variablesAndFiles;

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
            streampos lastPosition;
            if (!contentDispositionStringStream.eof()) {
                // Extract filename
                string filename;
                getline(contentDispositionStringStream, filename, ';');
                stringstream filenameStringStream(trim(filename));
                getline(filenameStringStream, filename, '=');
                getline(filenameStringStream, filename);
                filename = trim(filename);
                filename = filename.substr(1, filename.length() - 2);
                get<1>(variablesAndFiles)[name] = filename;

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
                // Extract variable
                while (line.compare("") != 0) {
                    getline(requestBodyStringStream, line);
                    line = trim(line);
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
                get<0>(variablesAndFiles)[name] = variable;
            }

            requestBodyStringStream.seekg(lastPosition);
        }
        else if (line.compare(boundary + "--") == 0) {
            // End boundary
            break;
        }
    }

    return variablesAndFiles;
}
