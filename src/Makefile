.PHONY: all clean scannerfiles

CC = gcc

CFLAGS  = -g -c -pedantic  

HISTORI_LIB = -L"./" -l"history5"
READLINE_LIB = -L"./" -l"readline5"

all: shell
	mkdir ../bin
	mv shell.exe ../bin 
	cp readline5.dll history5.dll shell_history.txt ../bin
	
shell: main.o parser.o scanner.o shell_system.o bricks.o logic.o
	$(CC) $(HISTORI_LIB) $(READLINE_LIB) -o shell main.o parser.o scanner.o bricks.o shell_system.o logic.o
	
main.o: main.c parser.h scanner.yy.h
	$(CC) $(CFLAGS) main.c 
	
logic.o: logic.c
	$(CC) $(CFLAGS) logic.c
	
bricks.o: bricks.c
	$(CC) $(CFLAGS) bricks.c 
	
shell_system.o: shell_system.c
	$(CC) $(CFLAGS) shell_system.c

scanner.o: scanner.yy.h scanner.yy.c
	$(CC) $(CFLAGS) scanner.yy.c -o scanner.o
	
scanner.yy.h: scannerfiles 

scanner.yy.c: scannerfiles

scannerfiles: scanner.l
	flex --outfile=scanner.yy.c --header-file=scanner.yy.h scanner.l
	
parser.o: parser.h parser.c

lemon: lemon.c
	$(CC) -o lemon lemon.c

parser.h parser.c: lemon parser.y
	./lemon.exe -l -c -s parser.y

clean:
	mv ../bin/shell_history.txt ./
	rm -R ../bin
	rm -f *.o
	rm -f scanner.yy.c scanner.yy.h
	rm -f parser.c parser.h parser.out
	rm -f shell.exe lemon.exe