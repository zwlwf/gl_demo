a.exe : a.c
	gcc a.c src/glad.c -Iinclude -Llib-mingw-w64 -lglfw3 -luser32 -lgdi32 -std=c99

b.exe : a.cpp
	g++ a.cpp glad.o -Iinclude -Llib-mingw-w64 -lglfw3 -luser32 -lgdi32 -std=c++11
