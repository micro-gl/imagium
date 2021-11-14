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

#include "token_parser.h"

class bundle {
public:
    using str=std::string;
    using dictionary=std::map<str, str>;
    using Key=const str &;
private:
    dictionary _dic;
public:
    static bundle fromTokens(const int &argc, char **argv) {
        return bundle{token_parser::parse(argc, argv)};
    }

    explicit bundle(dictionary dic) : _dic{std::move(dic)} { }
    explicit bundle(dictionary & dic) : _dic{(dic)} { }
    bundle(bundle &&bundle) noexcept : _dic{std::move(bundle._dic)} { }
    bundle(bundle & bundle) = default;
    bundle() = default;

    bool hasKey(const str &key) {
        return _dic.find(key)!=_dic.end();
    }

    int getValueAsInteger(Key key, int default_value=0) {
        const auto as_str=getValueAsString(key, std::to_string(default_value));
        return std::stoi(as_str);
    }

    float getValueAsFloat(Key key, float default_value=0.0f) {
        const auto as_str=getValueAsString(key, std::to_string(default_value));
        return std::stof(as_str);
    }

    bool getValueAsBoolean(Key key, bool default_value=false) {
        str bool_str_default=default_value?"true":"false";
        const auto as_str=getValueAsString(key, bool_str_default);
        return as_str=="true";
    }

    str getValueAsString(Key key, str default_value="") {
        const auto it= _dic.find(key);
        if(it==_dic.end()) return default_value;
        return it->second;
    }

    template <typename Value>
    void putValue(Key key, const Value & value) {
        _dic[key]=stringify<Value>(value);
    }

    template <typename Value>
    void removeKey(Key key) {
        const auto it= _dic.find(key);
        _dic.erase(it);
    }

    uint size() {
        return _dic.size();
    }

    void clear() {
        _dic.clear();
    }

private:
    template <typename Value>
    str stringify(const Value & value) { return std::to_string(value); }
    template <>
    str stringify<str>(const str & value) { return value; }
};
