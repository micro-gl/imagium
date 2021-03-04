cmake_minimum_required(VERSION 3.12)
project(imagium)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
message(${PROJECT_SOURCE_DIR})

# imagium lib target

set(SOURCES_LIB
        src/types.cpp
        src/bits.cpp
        src/stb_image.cpp
        src/lodepng.cpp
        )
add_library(imagium ${SOURCES_LIB})
target_include_directories(imagium
        PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src")
target_include_directories(imagium
        PUBLIC
        "${PROJECT_SOURCE_DIR}/include")

# imagium-cli target

set(SOURCES_CLI
        cli/utils.cpp
        cli/imagium-cli.cpp)
add_executable(imagium-cli ${SOURCES_CLI})
target_include_directories(imagium-cli
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(imagium-cli imagium)
set_target_properties(imagium-cli PROPERTIES OUTPUT_NAME "imagium")
add_custom_command(TARGET imagium-cli POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:imagium-cli>/assets)

# install target

install(TARGETS imagium-cli  RUNTIME DESTINATION "bin")
install(TARGETS imagium  LIBRARY DESTINATION "lib"
                         ARCHIVE DESTINATION "lib")
install(DIRECTORY include/imagium DESTINATION include/imagium)
