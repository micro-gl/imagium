# Imagium lib & Imagium-CLI
project contains two components  
- `imagium` is a c++ library, that processes images andre-shapes and packs/unpacks  
new pixels inside power of 2 data and returns `.c header files` strings out of them.  
- `imagium-cli` is a command line interface, that uses `imagium` to write `.c header files`    
export formats so you can embed/use them in your application.  

install **imagiumlib** and **cli** with ***CMake***
```bash
$ cd path/to/imagium
$ mkdir cmake-build-release
$ cd cmake-build-release
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ cmake --build . --target install
```

suppose it is installed at `<install-folder>`.

for using the `libimagium`, use:
```bash
add these to your Include directories
-I<install-folder>/include/imagium

add this path to your libs directories search
-L<install-folder>/lib/

add the lib to your link libs
-limagium
```
or you can skip `install` and use the lib from your cmake with
```cmake
# CMakeFiles.txt

# this will add the fontium lib target
add_subdirectory(path/to/imagium)

# then link
target_link_libraries(your-app imagium)
```

the `cli` target is installed at `<install-folder>/bin/imagium`, if it is global, then you can   
use it write away in your shell/terminal
```bash
$ imagium -h
```

### supported image formats
- `PNG`, `JPEG`, `BMP`, `TGA`, `GIF`, `PSD`, `PIC`, `PNM` are supported.
- for palette export only `PNG` is supported with embedded palette, that you  
have created in an image editing software.

### notes
- channels, that are reshaped into 0 bits are discarded in output to save storage.
- palette, is only supported for `png` format

## imagium lib
you can use the lib as follows
```c++
imagium::Imagium lib{};
Config config = Config::getDefault();
using bytearray = std::vector<unsigned char>;

// change the config
config.r=8; config.g=2; config.b=1; config.a=0;

// here you can load font data into the bytearray from disk or memory
bytearray image{};

// compute the c-array string
imagium::converter::result res = lib.produce(&image, config);
res.data; // is the bytearray result, currently represents a string
res.size_bytes; // represent what size in bytes the data will occupy in memory once loaded

```

## imagium cli guide

build `imagium-cli` target with `cmake` or install (look above for instructions)
```bash
$ cd imagium
$ mkdir cmake_build
$ cd cmake_build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ cmake --build . --target imagium-cli
$ cd ../bin
$ ./imagium -h
```

```text
usage:
  imagium <image file> [options]

description:
  imagium reshapes images and exports them into software arrays, so you can
  embed them in your program in the pixel format of your liking

options include:
  -rgba         value: R-G-B-A (literally)
                values are bits per channel
                example: 8-8-8-8 or 5-6-5-0 or 3-0-0-0 etc..
                note: 0 bits for a channel will completely discard the channel
  -unpack       if set, unpacks each channel separately inside a power of 2 number type = {r,g,b,a, r,g,b,a ...}
                if not set (default), packs all channels inside a power of 2 number type = {pix1, pix2, ...}
                notes: defaults to `true`, `false`, is desirable for 8|8|8|0 rgb config, where
                you will get the buffer = {r,g,b, r,g,b, .....} which is more optimal than packed.
  -indexed      create indexed data with embedded palette of the image
  -o            (optional) output name, if not set, will try to use file name
  -converter    (optional) value: converter-name
                choose a specific converter that you know of. by default,
                imagium will infer the correct one.
                example: regular_converter
  -h            show help

example:
  imagium foo.png -rgba 5-6-5
  imagium foo_with_16_color_palette.png -rgba 5-6-5 -indexed

Author:
  Tomer Shalev (tomer.shalev@gmail.com)
```
