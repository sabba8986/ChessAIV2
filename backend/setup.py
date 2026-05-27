from setuptools import Extension, setup 

debug = ["-g", "-O0", "-std=c++23", "-fno-omit-frame-pointer"]
release = ["-std=c++23", "-O3", "-flto", "-march=native"] 
setup(ext_modules=[Extension("board", ["Board.cpp", "interface.cpp", "Boardmodule.c", "attacks.cpp"], extra_compile_args = debug)])
