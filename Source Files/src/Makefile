
Risc-Verter 32: main.o functions.o formats.o
	gcc -g main.o functions.o formats.o -o 'Risc-Verter 32' -I./raylib/src -I -L./raylib/src -Wl,-rpath=/usr/local/lib64 -lraylib -lm

main.o: main.c functions.h formats.h raygui.h ../raylib/src/raylib.h
	gcc -g -c main.c

functions.o: functions.c functions.h formats.h raygui.h ../raylib/src/raylib.h gui_window_file_dialog.h
	gcc -g -c functions.c

formats.o: formats.c formats.h
	gcc -g -c formats.c

clean:
	rm *.o 'Risc-Verter 32'