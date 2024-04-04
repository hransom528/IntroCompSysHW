CC = gcc
CFLAGS = -Wall -O2

all: Project1 Project1Part2 genRandomVals

Project1: Project1.c
	$(CC) $(CFLAGS) Project1.c -o Project1
	chmod +x Project1

Project1Part2: Project1Part2.c
	$(CC) $(CFLAGS) Project1Part2.c -o Project1Part2
	chmod +x Project1Part2

genRandomVals: genRandomVals.c
	$(CC) $(CFLAGS) genRandomVals.c -o genRandomVals 
	chmod +x genRandomVals

clean:
	rm Project1 Project1Part2 genRandomVals
