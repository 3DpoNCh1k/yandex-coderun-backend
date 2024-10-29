cmake_minimum_required(VERSION 3.13)

project(yandex-coderun-backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(SANITIZERS)
    message(STATUS "Sanitize with ASAN and UBSAN")
    add_compile_options(-fsanitize=address,undefined -fno-sanitize-recover=all)
    add_link_options(-fsanitize=address,undefined)
endif()

include(FetchContent)

message(STATUS "Fetch algo")
FetchContent_Declare(
        algo
        GIT_REPOSITORY https://github.com/3DpoNCh1k/algo.git
        GIT_TAG master
)
FetchContent_MakeAvailable(algo)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

file(GLOB SOLUTIONS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/**/main.cpp)

foreach(SOLUTION_PATH IN LISTS SOLUTIONS)
    get_filename_component(SOLUTION ${SOLUTION_PATH} DIRECTORY)    
    
    add_executable(${SOLUTION} ${SOLUTION_PATH})
    target_link_libraries(${SOLUTION} algo)
endforeach()
