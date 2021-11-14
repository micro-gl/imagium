#pragma once
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
#include <map>
#include <cstdlib>
#include <utility>

namespace imagium {

    struct Config {

        using str=std::string;
        using strings=std::vector<str>;

        static
        Config getDefault() {
            Config config{};

            config.image_format="png";
            config.r=8;
            config.g=8;
            config.b=8;
            config.a=8;
            config.pack_channels=true;
            config.use_palette=false;
            config.converter="";
            config.output_name="test";

            return config;
        };

        str image_format;
        int r=-1, g=-1, b=-1, a=-1;
        bool pack_channels=true;
        bool use_palette=false;
        str converter="";
        str output_name="test";
    };
}
