cmake_minimum_required(VERSION 3.14)
project(openfst)

set(CMAKE_CXX_STANDARD 11)

include_directories(src/include)

file(GLOB_RECURSE openfst_lib_src src/lib/*.cc)
add_library(OPENFST_LIB ${openfst_lib_src})

file(GLOB_RECURSE openfst_script_src src/script/*.cc)
add_library(OPENFST_SCRIPT ${openfst_script_src})

file(GLOB_RECURSE openfst_bin_src src/bin/*.cc)
file(GLOB_RECURSE openfst_bin_src_main src/bin/*main.cc)
list(REMOVE_ITEM openfst_bin_src ${openfst_bin_src_main})

add_library(OPENFST_BIN ${openfst_bin_src_main})

foreach(item ${openfst_bin_src})
    get_filename_component(binary ${item} NAME_WE)
    add_executable(${binary} ${item})
    target_link_libraries(${binary} OPENFST_BIN OPENFST_LIB OPENFST_SCRIPT)
endforeach(item ${openfst_bin_src})
