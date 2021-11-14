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

#include <imagium/types.h>

namespace imagium {
        struct color_t {
            color_t(channel $r, channel $g, channel $b, channel $a,
                    bits $r_bits, bits $g_bits, bits $b_bits, bits $a_bits) :
                    r{$r}, g{$g}, b{$b}, a{$a}, r_bits{$r_bits}, g_bits{$g_bits},
                    b_bits{$b_bits}, a_bits{$a_bits} {};
            color_t(const color_t &val) :
                    color_t(val.r, val.g, val.b, val.a, val.r_bits, val.g_bits, val.b_bits,val.a_bits) {};
            color_t()= default;
            channel r=0, g=0, b=0, a=0;
            bits r_bits=8, g_bits=8, b_bits=8, a_bits=8;
            bits bits_rgba() const { return r_bits+g_bits+b_bits+a_bits; }
            bits bits_gba() const { return g_bits+b_bits+a_bits; }
            bits bits_ba() const { return b_bits+a_bits; }
            bits bits_a() const { return a_bits; }
        };
}
