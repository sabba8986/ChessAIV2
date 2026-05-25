from setuptools import Extension, setup 

setup(ext_modules=[Extension("board", ["Board.cpp", "interface.cpp", "Boardmodule.c", "attacks.cpp"], extra_compile_args = ["-std=c++23", "-O3", "-flto"])])
