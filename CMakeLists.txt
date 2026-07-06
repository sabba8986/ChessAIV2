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


install(TARGETS board_interface LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})
