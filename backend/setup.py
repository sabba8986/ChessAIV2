from setuptools import Extension, setup 

debug = ["-g", "-O0", "-std=c++23", "-fno-omit-frame-pointer", "-fno-inline"]
release = ["-std=c++23", "-O3", "-flto", "-march=native"] 
setup(ext_modules=[Extension("board", ["board.cpp", "interface.cpp", "boardmodule.c", "attacks.cpp"], extra_compile_args = debug)])
