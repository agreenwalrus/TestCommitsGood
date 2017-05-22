.PHONY: all clean scannerfiles

CC = gcc

CFLAGS  = -g -Wall -pedantic

all: shell
	
shell: main.o parser.o scanner.o shell_system.o
	$(CC) -o shell main.o parser.o scanner.o
	
main.o: main.c parser.h scanner.yy.h
	$(CC) -c main.c 
	
shell_system.o: shell_system.c
	$(CC) -c shell_system.c

scanner.o: scanner.yy.h scanner.yy.c
	$(CC) -c scanner.yy.c -o scanner.o
	
scanner.yy.h: scannerfiles 

scanner.yy.c: scannerfiles

scannerfiles: scanner.l
	flex --outfile=scanner.yy.c --header-file=scanner.yy.h scanner.l
	
parser.o: parser.h

lemon: lemon.c
	$(CC) -o lemon lemon.c

parser.h: lemon parser.y
	./lemon.exe -l -c -s parser.y		

clean:
	rm -f *.o
	rm -f scanner.yy.c scanner.yy.h
	rm -f parser.c parser.h parser.out
	rm -f shell.exe lemon.exe