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
#include <imagium/bits.h>

namespace imagium {
    channel convert_channel(channel val, bits bits_from, bits bits_to) {
        return bits_from==0 ? 0 : (val*((int(1)<<bits_to)-1))/((int(1)<<bits_from)-1);
    }

    str infer_type_needed_for_bits(bits val) {
        if(val<=8) return "uint8_t";
        else if(val<=16) return "uint16_t";
        else if(val<=32) return "uint32_t";
        else if(val<=64) return "uint64_t";
        return "uint64_t";
    }

    bits infer_bits_used_from_unsigned_byte_value(ubyte val) {
        if(val<1) return 0;
        else if(val<2) return 1;
        else if(val<4) return 2;
        else if(val<8) return 3;
        else if(val<16) return 4;
        else if(val<32) return 5;
        else if(val<64) return 6;
        else if(val<128) return 7;
        return 8;
    }

    ubyte infer_power_of_2_bits_needed_from_bits(bits bits_val) {
        if(bits_val<=1) return 1;
        else if(bits_val<=2) return 2;
        else if(bits_val<=4) return 4;
        else if(bits_val<=8) return 8;
        else if(bits_val<=16) return 16;
        else if(bits_val<=32) return 32;
        else if(bits_val<=64) return 64;
        return 64;
    }

    ubyte infer_power_of_2_bytes_needed_from_bits(bits bits_val) {
        if(bits_val<=8) return 1;
        else if(bits_val<=16) return 2;
        else if(bits_val<=32) return 4;
        else if(bits_val<=64) return 8;
        return 8;
    }

    ubyte infer_power_of_2_bytes_needed_for_rgba(bits r, bits g, bits b, bits a) {
        bits sum =r+g+b+a;
        return infer_power_of_2_bytes_needed_from_bits(sum);
    }

    ubyte infer_power_of_2_bits_needed_for_rgba(bits r, bits g, bits b, bits a) {
        bits sum =r+g+b+a;
        return infer_power_of_2_bits_needed_from_bits(sum);
    }

}
