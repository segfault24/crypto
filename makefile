CC=gcc
CFLAGS=-c -Wall

all: main.out

main.out: main.o mycrypt.o mymd5.o mysha256.o
	gcc main.o mycrypt.o mymd5.o mysha256.o -o main.out

main.o: main.c
	$(CC) $(CFLAGS) main.c

mymd5.o: mymd5.c
	$(CC) $(CFLAGS) mymd5.c

mysha256.o: mysha256.c
	$(CC) $(CFLAGS) mysha256.c

mycrypt.o: mycrypt.c
	$(CC) $(CFLAGS) mycrypt.c

clean:
	rm -f main.out *.o *~

