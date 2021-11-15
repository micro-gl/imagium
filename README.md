# Imagium lib & Imagium-CLI
Project contains two components  
- `imagium` is a C++ library, that processes images and re-shapes and packs/unpacks  
new pixels inside power of 2 data and returns `.c header files` strings out of them.  
- `imagium-cli` is a command line interface, that uses `imagium` to write `.c header files`    
export formats so you can embed/use them in your application.  

## Installing  **`imagiumlib`** and **cli**
```bash
$ cd path/to/imagium
$ mkdir cmake-build-release
$ cd cmake-build-release
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ cmake --build . --target install
```
The `cli` target is installed at `<install-folder>/bin/imagium`, if it is global, then you can   
use it right away in your shell/terminal
```bash
$ imagium -h
```

## Consuming `imagiumlib` in your project
Following options are available:
1. Copy the project to a sub folder of your project. inside your **`CMakeLists.txt`** add
```cmake
add_subdirectory(/path/to/imagium)
target_link_libraries(your_app imagium)
```
2. If you installed **`imagium`** (see above) at your system, you can instead
```cmake
find_package(imagium CONFIG REQUIRED)
target_link_libraries(your_app imagium::imagium)
```

## supported image formats
- `PNG`, `JPEG`, `BMP`, `TGA`, `GIF`, `PSD`, `PIC`, `PNM` are supported.
- For palette export only `PNG` is supported with embedded palette, that you  
have created in an image editing software.

## notes
- Channels, that are reshaped into 0 bits are discarded in output to save storage.
- Palette, is only supported for `png` format

## imagium lib
You can use the lib as follows
```c++
imagium::Imagium lib{};
Config config = Config::getDefault();
using bytearray = std::vector<unsigned char>;

// change the config
config.r=8; config.g=2; config.b=1; config.a=0; config.image_format="png";

// here you can load raw image data into the bytearray from disk or memory
bytearray image{};

// compute the c-array string
imagium::converter::result res = lib.produce(&image, config);
res.data; // is the bytearray result, currently represents a string
res.size_bytes; // represent what size in bytes the data will occupy in memory once loaded

```

## imagium cli guide

Build `imagium-cli` target with `cmake` or install (look above for instructions)
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
