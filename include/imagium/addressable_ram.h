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

    /**
     * addressable RAM is an interface for a continuous chunk of memory,
     * the chunk is composed of atomic blocks of memory of any size and
     * on top of it we lay elements, that might have different sizes of blocks,
     * think of it like tesselation. suppose, we have array of bytes and we want it
     * to store elements of 4bits size for example. Therefore, addressing the i'th
     * element may not correspond directly to the i'th of the array.
     */
    class addressable_ram {
    public:
        virtual void write(unsigned index, uint64_t value) = 0;
        virtual uint64_t operator[](int index) const = 0;
        virtual uint bits_per_element() const = 0;
        virtual uint bits_per_storage_type() const = 0;
        virtual uint elementsCount() const = 0;
        virtual uint storageCount() const = 0;
        virtual str toString(const str &name) const = 0;
    };

}
