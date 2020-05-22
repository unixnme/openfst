cmake_minimum_required(VERSION 3.14)
project(openfst)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,dynamic_lookup")

option(ENABLE_FAR "enable far support" OFF)
option(ENABLE_NGRAM "enable ngram fst support" OFF)

file(GLOB_RECURSE openfst_lib_src src/lib/*.cc)

file(GLOB_RECURSE openfst_script_src src/script/*.cc)
list(APPEND openfst_lib_src ${openfst_script_src})

if(ENABLE_FAR)
    file(GLOB_RECURSE openfst_far_src src/extensions/far/*.cc)
    file(GLOB_RECURSE openfst_far_src_main src/extensions/far/*main.cc)
    list(APPEND openfst_lib_src ${openfst_far_src})
endif(ENABLE_FAR)

if(ENABLE_NGRAM)
    file(GLOB_RECURSE openfst_ngram_src src/extensions/ngram/*.cc)
    list(APPEND openfst_lib_src ${openfst_ngram_src})
endif(ENABLE_NGRAM)

add_library(OPENFST_LIB SHARED ${openfst_lib_src})
target_include_directories(OPENFST_LIB PUBLIC src/include)

file(GLOB_RECURSE openfst_bin_src src/bin/*.cc)
file(GLOB_RECURSE openfst_bin_src_main src/bin/*main.cc)
list(REMOVE_ITEM openfst_bin_src ${openfst_bin_src_main})

foreach(item ${openfst_bin_src})
    get_filename_component(binary ${item} NAME_WE)
    add_executable(${binary} ${item} src/bin/${binary}-main.cc)
    target_link_libraries(${binary} OPENFST_LIB)
endforeach(item ${openfst_bin_src})

add_executable(rho_compose rho_compose/rho_compose.cc)
target_link_libraries(rho_compose OPENFST_LIB)