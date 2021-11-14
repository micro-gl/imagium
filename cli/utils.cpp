/*========================================================================================
 Copyright (2021), Tomer Shalev (tomer.shalev@gmail.com, https://github.com/HendrixString).
 All Rights Reserved.
 License is a custom open source semi-permissive license with the following guidelines:
 1. unless otherwise stated, derivative work and usage of this file is permitted and
    should be credited to the project and the author of this project.
 2. Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
========================================================================================*/
#include "utils.h"
#include <utility>
#include <iostream>

std::vector<unsigned char> * loadFileAsByteArray(const std::string &file_name) {
    std::ifstream ifs(file_name, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    auto isGood = ifs.good();
    if(!isGood)
        throw std::runtime_error("error loading file - " + file_name);
    auto length = static_cast<size_t>(ifs.tellg());
    auto *ret = new std::vector<unsigned char>(length);
    ifs.seekg(0, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(ret->data()), length);
    ifs.close();
    return ret;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    std::vector<std::string> list;
    split(std::move(s), std::move(delimiter), list);
    return list;
}

void split(std::string s, const std::string& delimiter, std::vector<std::string> &list) {
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
}
