
assembler: main.o functions.o formats.o
	gcc main.o functions.o formats.o -o assembler

main.o: main.c functions.h formats.h
	gcc -c main.c

functions.o: functions.c functions.h formats.h
	gcc -c functions.c

formats.o: formats.c formats.h
	gcc -c formats.c

clean:
	rm *.o output