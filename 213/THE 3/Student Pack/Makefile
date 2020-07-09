CC=g++
CFLAGS=-c -ansi -Wall -pedantic-errors -O0

all: main

main: main_test3.o Article.o Machine.o
	$(CC) Machine.o Article.o main_test3.o -o article

Machine.o: Machine.cpp
	$(CC) $(CFLAGS) Machine.cpp

Article.o: Article.cpp
	$(CC) $(CFLAGS) Article.cpp

main_test3.o: main_test3.cpp
	$(CC) $(CFLAGS) main_test3.cpp

clean:
	rm *o
