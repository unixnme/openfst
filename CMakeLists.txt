cmake_minimum_required(VERSION 3.14)
project(openfst)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE openfst_lib_src src/lib/*.cc)

file(GLOB_RECURSE openfst_script_src src/script/*.cc)
list(APPEND openfst_lib_src ${openfst_script_src})

file(GLOB_RECURSE openfst_bin_src src/bin/*.cc)
file(GLOB_RECURSE openfst_bin_src_main src/bin/*main.cc)
list(REMOVE_ITEM openfst_bin_src ${openfst_bin_src_main})
list(APPEND openfst_lib_src ${openfst_bin_src_main})

add_library(OPENFST_LIB ${openfst_lib_src})
target_include_directories(OPENFST_LIB PUBLIC src/include)

foreach(item ${openfst_bin_src})
    get_filename_component(binary ${item} NAME_WE)
    add_executable(${binary} ${item})
    target_link_libraries(${binary} OPENFST_LIB)
endforeach(item ${openfst_bin_src})

add_executable(read read.cc)
target_link_libraries(read OPENFST_LIB)