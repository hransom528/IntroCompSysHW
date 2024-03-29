CC = gcc
CFLAGS = -Wall -O2

all: Project1 genRandomVals

Project1: Project1.c
	$(CC) $(CFLAGS) Project1.c -o Project1
	chmod +x Project1

genRandomVals: genRandomVals.c
	$(CC) $(CFLAGS) genRandomVals.c -o genRandomVals 
	chmod +x genRandomVals

clean:
	rm Project1 genRandomVals
