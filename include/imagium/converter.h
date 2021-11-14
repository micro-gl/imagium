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

#include <iostream>
#include <imagium/types.h>
#include <imagium/Config.h>

namespace imagium {

    class converter {
    public:
        struct result {
            byte_array data;
            uint size_bytes;
        };

        converter() = default;

//        virtual std::vector<ubyte> write(byte_array * $data, const options & options) const = 0;
        virtual result write(byte_array * $data, const Config & options) const = 0;
    };
}
