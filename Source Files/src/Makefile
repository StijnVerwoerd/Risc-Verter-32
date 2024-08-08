.PHONY: clean

# Default target for Linux build
Linux: main.o functions.o formats.o
	gcc -g main.o functions.o formats.o -o 'Risc-Verter 32' -I./raylib/src -L./raylib/src -Wl,-rpath=/usr/local/lib64 -lraylib -lm

# Object file rules for Linux
main.o: main.c functions.h formats.h ../raygui_edit/raygui.h ../raylib/src/raylib.h
	gcc -g -c main.c

functions.o: functions.c functions.h formats.h ../raygui_edit/raygui.h ../raylib/src/raylib.h ../raygui_edit/gui_window_file_dialog.h
	gcc -g -c functions.c

formats.o: formats.c formats.h
	gcc -g -c formats.c

# Clean target
clean:
	rm *.o 'Risc-Verter 32' 'Risc-Verter 32.exe'


