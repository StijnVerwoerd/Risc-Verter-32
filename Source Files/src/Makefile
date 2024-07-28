
assembler: main.o functions.o formats.o
	gcc -g main.o functions.o formats.o -o assembler -I./raylib/src -I./raygui/src -L./raylib/src -Wl,-rpath=/usr/local/lib64 -lraylib -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon -lEGL -lGL -lm -ldl -lrt -lpthread

main.o: main.c functions.h formats.h ../raygui/src/raygui.h ../raylib/src/raylib.h
	gcc -c main.c

functions.o: functions.c functions.h formats.h ../raygui/src/raygui.h ../raylib/src/raylib.h
	gcc -c functions.c

formats.o: formats.c formats.h
	gcc -c formats.c

clean:
	rm *.o assembler