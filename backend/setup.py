from setuptools import Extension, setup 

setup(ext_modules=[Extension("board", ["Board.cpp", "interface.cpp", "Boardmodule.c"], extra_compile_args = ["-std=c++23"])])
