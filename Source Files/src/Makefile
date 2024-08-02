
assembler: main.o functions.o formats.o
	gcc -g main.o functions.o formats.o -o assembler -I./raylib/src -I./raygui/src -L./raylib/src -Wl,-rpath=/usr/local/lib64 -lraylib -lm

main.o: main.c functions.h formats.h ../raygui/src/raygui.h ../raylib/src/raylib.h
	gcc -g -c main.c

functions.o: functions.c functions.h formats.h ../raygui/src/raygui.h ../raylib/src/raylib.h ../raygui/src/gui_window_file_dialog.h
	gcc -g -c functions.c

formats.o: formats.c formats.h
	gcc -g -c formats.c

clean:
	rm *.o assembler assemblerdebug