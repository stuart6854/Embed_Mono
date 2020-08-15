cmake_minimum_required(VERSION 3.18)
project(Embed_Mono)

set(CMAKE_CXX_STANDARD 20)

#set(MONO_COMPILER_FLAGS "pkg-config --cflags mono-2")
#set(MONO_LINKER_FLAGS "pkg-config --libs mono-2")

find_package(PkgConfig REQUIRED)
pkg_check_modules(Mono IMPORTED_TARGET mono-2)

add_executable(Embed_Mono src/main.cpp)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        include
)

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        PkgConfig::Mono
)

#target_compile_options(
#        ${PROJECT_NAME}
#        PRIVATE
#        ${MONO_COMPILER_FLAGS}
#)
#
#target_link_options(
#        ${PROJECT_NAME}
#        PRIVATE
#        ${MONO_LINKER_FLAGS}
#)