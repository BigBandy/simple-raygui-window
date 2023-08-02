# NOTE: For Windows only. Will require tinkering for other platforms. If unsure, use raylib makefile homie.

default:
	gcc ./src/*.c -o gui_test.exe -fdiagnostics-color=always -O2 -I C:\Users\Andy\source\repos\raylib\src -L C:\Users\Andy\source\repos\raylib\src -static -lraylib -lopengl32 -lgdi32 -lwinmm