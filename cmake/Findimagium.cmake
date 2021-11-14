#========================================================================================
# Copyright (2021), Tomer Shalev (tomer.shalev@gmail.com, https://github.com/HendrixString).
# All Rights Reserved.
#
# this should help find the imagium headers-only package and define the imagium target that was
# installed on your system and does not include CMakeLists.txt file, so you can easily link to it.
# If successful, the following will happen:
# 1. imagium_FOUND will be defined
# 1. imagium_INCLUDE_DIR will be defined
# 1. imagium_LIBRARY will be defined
# 2. imagium::imagium target will be defined so you can link to it using target_link_libraries(..)
#========================================================================================
include(GNUInstallDirs)
include(FindPackageHandleStandardArgs)

find_path(imagium_INCLUDE_DIR
        NAMES imagium
        HINTS ${CMAKE_INSTALL_INCLUDEDIR}
        PATH_SUFFIXES imagium.h)
find_library(imagium_LIBRARY
        NAMES imagium
        HINTS ${CMAKE_INSTALL_LIBDIR})

find_package_handle_standard_args(imagium DEFAULT_MSG
        imagium_LIBRARY imagium_INCLUDE_DIR)

if(imagium_FOUND)
    message("imagium was found !!!")
else(imagium_FOUND)
    message("imagium was NOT found !!!")
endif(imagium_FOUND)

if(imagium_FOUND AND NOT TARGET imagium::imagium)
    # build the target
    add_library(imagium::imagium STATIC IMPORTED)
    set_target_properties(
            imagium::imagium
            PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${imagium_INCLUDE_DIR}"
            IMPORTED_LOCATION ${imagium_LIBRARY})
endif()