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
    channel convert_channel(channel val, bits bits_from, bits bits_to);
    str infer_type_needed_for_bits(bits val);
    bits infer_bits_used_from_unsigned_byte_value(ubyte val);
    ubyte infer_power_of_2_bytes_needed_from_bits(bits bits_val);
    ubyte infer_power_of_2_bytes_needed_for_rgba(bits r, bits g, bits b, bits a=0);
    ubyte infer_power_of_2_bits_needed_from_bits(bits bits_val);
    ubyte infer_power_of_2_bits_needed_for_rgba(bits r, bits g, bits b, bits a=0);

}