CC = g++
CFLAGS = -c -Wall -g

all: Test

Test: main.o
	$(CC) main.o -o Test

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *.o Test