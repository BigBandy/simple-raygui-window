# NOTE: For Windows only. Will require tinkering for other platforms. If unsure, use raylib makefile homie.

raylib_path ?= C:\Users\Andy\source\repos\raylib\src

default:
	gcc ./src/*.c -o gui_test.exe -fdiagnostics-color=always -O2 -I $(raylib_path) -L $(raylib_path) -static -lraylib -lopengl32 -lgdi32 -lwinmm