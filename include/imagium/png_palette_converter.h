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

#include "converter.h"
#include "typed_packed_array.h"
#include "bits.h"
#include "color_t.h"
#include "utils.h"
#include "lodepng.h"
#include <exception>

namespace imagium {

    class png_palette_converter : public converter {
    public:
        png_palette_converter()= default;

        result write(byte_array * data, const Config & config) const override {
            uint width, height, input_channels=4, bits_depth=8;
            bits r_bits=config.r, g_bits=config.g, b_bits=config.b, a_bits=config.a;
            uint out_channels= (r_bits?1:0)+(g_bits?1:0)+(b_bits?1:0)+(a_bits?1:0);

            std::vector<unsigned char> image; //the raw pixels
            lodepng::State state; //optionally customize this one
            // request a palette on the output
            state.info_raw.colortype=LodePNGColorType::LCT_PALETTE;
            unsigned error = lodepng::decode(image, width, height, state, *data);
            bits_depth=state.info_png.color.bitdepth;
            ubyte * palette=state.info_png.color.palette;
            ubyte palette_number_of_pixels=state.info_png.color.palettesize;
            if(error)
                throw std::runtime_error("png_palette_converter error "+ std::to_string(error)+
                ": "+ lodepng_error_text(error));

            const unsigned pixels_count = width*height;
            addressable_ram * index_array= nullptr, *palette_array=nullptr;

            { // palette
                auto bits_needed= infer_power_of_2_bits_needed_from_bits(r_bits+g_bits+b_bits+a_bits);
                uint how_many= palette_number_of_pixels;
                if(!config.pack_channels) {
                    bits_needed=std::max<bits>({r_bits, g_bits, b_bits, a_bits});
                    bits_needed=infer_power_of_2_bytes_needed_from_bits(bits_needed)*8;
                    how_many=palette_number_of_pixels*out_channels;
                }
                palette_array=factory_PackedArray::getArray(how_many, bits_needed);

                for (unsigned ix = 0, jx=0; ix < palette_number_of_pixels; ++ix, jx+=input_channels) {
                    uint r=convert_channel(palette[jx+0], bits_depth, r_bits);
                    uint g=convert_channel(palette[jx+1], bits_depth, g_bits);
                    uint b=convert_channel(palette[jx+2], bits_depth, b_bits);
                    uint a=convert_channel(palette[jx+3], bits_depth, a_bits);
                    color_t color{r, g, b, a, r_bits, g_bits, b_bits, a_bits};
                    if(config.pack_channels) {
                        uint64_t val=pack_bits_in_number(color);
                        palette_array->write(ix, val);
                    } else {
                        unsigned idx2=ix*out_channels;
                        unsigned acc=0;
                        if(r_bits!=0) palette_array->write(idx2+acc++, color.r);
                        if(g_bits!=0) palette_array->write(idx2+acc++, color.g);
                        if(b_bits!=0) palette_array->write(idx2+acc++, color.b);
                        if(a_bits!=0) palette_array->write(idx2+acc++, color.a);
                    }
                }
            }
            { // index map
                unsigned bpp=infer_bits_used_from_unsigned_byte_value(palette_number_of_pixels-1);
                bpp=infer_power_of_2_bits_needed_from_bits(bpp);
                index_array=factory_PackedArray::getArray(width*height, bpp);
                for (unsigned ix = 0; ix < pixels_count; ++ix) {
                    // lode png serves unpacked values for indices, so we can pack them
                    index_array->write(ix, image[ix]);
                }
            }
            str all;
            { // generate strings
                const str include_str= "#include <cstdint>\n\n// this file was created by Imagium CLI \n\n";

                const str palette_string= palette_array->toString(config.output_name + "_palette");
                const str palette_comment= generate_comment(palette_number_of_pixels, 1,
                                                            palette_array->bits_per_element(),
                                                            palette_array->bits_per_storage_type(),
                                                            r_bits, g_bits, b_bits, a_bits, config.pack_channels);
                const str indexed_string= index_array->toString(config.output_name);
                const str index_comment= generate_comment(width, height,
                                                          index_array->bits_per_element(),
                                                          index_array->bits_per_storage_type(),
                                                          index_array->bits_per_element(), 0, 0, 0,
                                                          true);
                all=include_str+palette_comment + "\n" + palette_string + "\n\n" +
                        index_comment + "\n" + indexed_string + "\n";

            }
            byte_array result_arr{all.begin(), all.end()};
            uint bytes=((index_array->storageCount() * index_array->bits_per_storage_type()) / 8) +
                        ((palette_array->storageCount() * palette_array->bits_per_storage_type()) / 8);
            result export_{result_arr, bytes};
            return export_;
        };

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

        uint64_t pack_bits_in_number(const color_t &color) const {
            uint64_t storage = (color.r << color.bits_gba()) +
                               (color.g << color.bits_ba()) +
                               (color.b << color.bits_a()) +
                               (color.a << 0);
            return storage;
        }

    };
}
