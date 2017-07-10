OBJS = main.o parser.o lexer.o
DEBUG = -g
CC = gcc
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: phun

phun: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o phun
   
main.o: main.c phun.h
	$(CC) $(DEBUG) $(CFLAGS) main.c
parser.o: parser.c phun.h
	$(CC) $(DEBUG) $(CFLAGS) parser.c
lexer.o: lexer.c phun.h
	$(CC) $(DEBUG) $(CFLAGS) lexer.c
    
phun.h:
	touch *.c

clean:
	rm *.o phun
