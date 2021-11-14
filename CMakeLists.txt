cmake_minimum_required(VERSION 3.12)
project("imagium"
        VERSION
        1.0.0
        DESCRIPTION
        "processes images and re-shapes and packs/unpacks new pixels inside power of 2 data and returns .c header files"
        HOMEPAGE_URL
        "https://micro-gl.github.io/docs")
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

message("===============\n${PROJECT_NAME} v${PROJECT_VERSION} project \n===============")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# imagium lib target
set(SOURCES_LIB
        src/types.cpp
        src/bits.cpp
        src/stb_image.cpp
        src/lodepng.cpp
        )
add_library(${PROJECT_NAME} STATIC ${SOURCES_LIB})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
        PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src")
target_include_directories(
        ${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
# install cmake config files for targets of this lib, so they can be found more easily
# without a custom find script, but with find_package(micro-alloc CONFIG) command
install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
# install include files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# imagium-cli target

set(SOURCES_CLI
        cli/utils.cpp
        cli/imagium-cli.cpp)
add_executable(imagium-cli ${SOURCES_CLI})
target_include_directories(imagium-cli
        PRIVATE
        ${PROJECT_SOURCE_DIR}/cli
        ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(imagium-cli imagium)
set_target_properties(imagium-cli PROPERTIES OUTPUT_NAME "imagium")
add_custom_command(TARGET imagium-cli POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:imagium-cli>/assets)
install(TARGETS imagium-cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
