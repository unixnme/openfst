cmake_minimum_required(VERSION 3.14)
project(openfst)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -O0")

include_directories(src/include)

file(GLOB_RECURSE openfst_src src/lib/*.cc)
add_library(OPENFST ${openfst_src})

file(GLOB_RECURSE openfst_script src/script/*.cc)
add_library(OPENFST_SCRIPT ${openfst_script})

file(GLOB_RECURSE openfst_bin src/bin/*.cc)
file(GLOB_RECURSE openfst_bin_main src/bin/*main.cc)
list(REMOVE_ITEM openfst_bin ${openfst_bin_main})

add_library(OPENFST_BINARY ${openfst_bin_main})

foreach(item ${openfst_bin})
    get_filename_component(binary ${item} NAME_WE)
    add_executable(${binary} ${item})
    target_link_libraries(${binary} OPENFST_BINARY OPENFST OPENFST_SCRIPT)
endforeach(item ${openfst_bin})

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

print_all_variables()
#foreach(file ${openfst_bin})
#    add_executable()
#add_executable(fstcompile src/bin/fstcompile.cc src/bin/fstcompile-main.cc)
#target_link_libraries(fstcompile OPENFST OPENFST_SCRIPT)