task3: main.o liststr.o
	gcc -Wall -g0 main.o liststr.o -o task3

main.o: main.c liststr.h
	gcc -Wall -g0 -c main.c

liststr.o: liststr.c liststr.h
	gcc -Wall -g0 -c liststr.c

clean:
	rm -f main.o liststr.o task3

all: task3
