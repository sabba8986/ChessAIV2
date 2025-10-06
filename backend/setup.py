from setuptools import setup, Extension

api_module = Extension('board', sources=['Boardmodule.c', 'interface.cpp', 'Board.cpp'], extra_compile_args=['-std=c++20'], language = 'c++')

setup(name='board', version='1.0', description = "An interface to the ChessV2 game engine.", 
      ext_modules=[api_module])

