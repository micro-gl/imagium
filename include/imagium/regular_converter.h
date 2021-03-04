#pragma once

#include <exception>
#include <stb_image/stb_image.h>
#include <lodepng/lodepng.h>
#include "converter.h"
#include "typed_packed_array.h"
#include "bits.h"
#include "color_t.h"
#include "utils.h"

namespace imagium {

    class regular_converter : public converter {
    public:
        regular_converter()= default;

        result write(byte_array * data, const Config & config) const override {
            int width, height, input_channels=4, bits_depth=8, out_channels=0;
            bits r_bits=config.r, g_bits=config.g, b_bits=config.b, a_bits=config.a;
            out_channels= (r_bits?1:0)+(g_bits?1:0)+(b_bits?1:0)+(a_bits?1:0);
            ubyte * image=nullptr;
            {
                int nrChannels;
                image=stbi_load_from_memory(data->data(), data->size(), &width, &height,
                        &nrChannels, input_channels);
            }
            const unsigned pixels_count = width*height;
            addressable_ram * array= nullptr;
            {
                auto bits_needed= infer_power_of_2_bits_needed_from_bits(r_bits+g_bits+b_bits+a_bits);
                uint how_many= width*height;
                if(!config.pack_channels) {
                    bits_needed=std::max<bits>({r_bits, g_bits, b_bits, a_bits});
                    // round to next byte storage unit, this is important, so the packing
                    // array will not compress
                    bits_needed=infer_power_of_2_bytes_needed_from_bits(bits_needed)*8;
                    how_many=width*height*out_channels;
                }
                array=factory_PackedArray::getArray(how_many, bits_needed);
            }

            for (unsigned ix = 0, jx=0; ix < pixels_count; ++ix, jx+=input_channels) {
                uint r=convert_channel(image[jx+0], bits_depth, r_bits);
                uint g=convert_channel(image[jx+1], bits_depth, g_bits);
                uint b=convert_channel(image[jx+2], bits_depth, b_bits);
                uint a=convert_channel(image[jx+3], bits_depth, a_bits);
                color_t color{r, g, b, a, r_bits, g_bits, b_bits, a_bits};

                if(config.pack_channels) {
                    uint64_t val=pack_bits_in_number(color);
                    array->write(ix, val);
                } else {
                    unsigned idx2=ix*out_channels;
                    unsigned acc=0;
                    if(r_bits!=0) array->write(idx2+acc++, color.r);
                    if(g_bits!=0) array->write(idx2+acc++, color.g);
                    if(b_bits!=0) array->write(idx2+acc++, color.b);
                    if(a_bits!=0) array->write(idx2+acc++, color.a);
                }
            }

            const str include_str= "#include <cstdint>\n\n// this file was created by Imagium CLI \n\n";
            const str rendered_string= array->toString(config.output_name);
            const str comment= generate_comment(width, height, array->bits_per_element(), array->bits_per_storage_type(),
                                                r_bits, g_bits, b_bits, a_bits, config.pack_channels);
            const str result_string= include_str + comment + "\n" + rendered_string;
            byte_array result_array {result_string.begin(), result_string.end()};
            uint bytes=(array->storageCount() * array->bits_per_storage_type())/8;
            result export_{result_array, bytes};

            return export_;
        };

        uint64_t pack_bits_in_number(const color_t &color) const {
            uint64_t storage = (color.r << color.bits_gba()) +
                             (color.g << color.bits_ba()) +
                             (color.b << color.bits_a()) +
                             (color.a << 0);
            return storage;
        }

        static
        str generate_comment(int w, int h, bits element_bits, bits storage_bits,
                             bits r, bits g, bits b, bits a, bool isPacked) {
            str result;
            char arr[250];
            if(isPacked) {
                uint elements_per_storage=storage_bits/element_bits;
                sprintf(arr, "// %dx%d pixels in this array. each pixel is bit packed (%d|%d|%d|%d) "
                             "inside %d bits block, \n// laid in a %d bits of array storage. "
                             "Each array element contains %d pixels", w,h,r,g,b,a, element_bits,
                        storage_bits, elements_per_storage);
            } else {
                sprintf(arr, "// %dx%d pixels in this array. each channel of pixel is un-packed "
                             "(%d|%d|%d|%d) inside %d bits block of array storage. "
                             "\n// [r,g,b,a, r,g,b,a , .....], note 0 bit channels are removed",
                        w,h, r,g,b,a, storage_bits);
            }
            result=str{arr};
            return result;
        }

    };
}
