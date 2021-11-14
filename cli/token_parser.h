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
#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>

class token_parser {
private:
    using str=std::string;
    using tokens=std::vector<str>;
    using dictionary=std::map<str, str>;

public:
    token_parser()= delete;

    static
    dictionary parse (const int &argc, char **argv){
        dictionary dic{};
        int index=1;
        str waiting_key="VOID_KEY";
        str aggregate_values;

        while(index<argc) {
            const str token=str{argv[index]};
            const bool is_key= isKey(token);
            if(is_key) {
                //std::cout << waiting_key << " = " << aggregate_values << std::endl;
                dic[waiting_key]=aggregate_values;
                waiting_key=token.substr(1);
                aggregate_values="";
            } else { // is a value
                str space=aggregate_values.empty() ? "" : " ";
                aggregate_values += space + token;
            }
            index++;
        }
        dic[waiting_key]=aggregate_values;
        // std::cout << waiting_key << " = " << aggregate_values << std::endl;
        return dic;
    }

    static
    bool isKey(const str & val) {
        if(val.empty()) return false;
        return val[0]=='-';
    }

};