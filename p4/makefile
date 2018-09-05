CFLAGS= -Wall -std=c++14 -g -O0 -pedantic-errors -c
LFLAGS= -Wall

all: shell

shell: shell.o
	g++ $(LFLAGS) -o shell shell.o
	
shell.o: shell.cpp
	g++ $(CFLAGS) shell.cpp

clean:
	rm -f *.cpp~ *.o shell