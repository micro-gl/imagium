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

#include <cstdint>
#include <vector>
#include <string>

namespace imagium {

    using uint= unsigned int;
    using ubyte= unsigned char;
    using channel= uint32_t ;
    using bits= unsigned char;
    using byte_array= std::vector<ubyte>;
    using str= std::string;

    enum class image_format {
        PNG, JPEG, BMP, unknown
    };

    enum class color_format {
        true_color,
        palette,
        grayscale,
        unknown
    };

    std::string image_format_to_string(const image_format & val);
    std::string color_format_to_string(const color_format & val);
    image_format string_to_image_format(const str &val);
    color_format string_to_color_format(const str &val);
}
