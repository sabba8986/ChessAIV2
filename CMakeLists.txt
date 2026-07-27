cmake_minimum_required(VERSION 3.23)
project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(PYBIND11_FINDPYTHON ON)


find_package(pybind11 CONFIG REQUIRED HINTS env/lib/python3.14/site-packages/pybind11)

pybind11_add_module(board_interface backend/${SKBUILD_PROJECT_NAME}/boardmodule.cpp 
                          backend/${SKBUILD_PROJECT_NAME}/board.cpp 
                          backend/${SKBUILD_PROJECT_NAME}/history.cpp 
                          backend/${SKBUILD_PROJECT_NAME}/move_list.cpp 
                          backend/${SKBUILD_PROJECT_NAME}/undo_move.cpp 
                          backend/${SKBUILD_PROJECT_NAME}/move.cpp
                          backend/${SKBUILD_PROJECT_NAME}/bitboard_moves.cpp
                          backend/${SKBUILD_PROJECT_NAME}/board_state.cpp)


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(board_interface PRIVATE -fconstexpr-ops-limit=10000000)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
    target_compile_options(board_interface PRIVATE -fconstexpr-steps=10000000)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(board_interface PRIVATE /constexpr:steps10000000)
endif()


install(TARGETS board_interface LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})
