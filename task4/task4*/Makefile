all: echo pwd ls mv cp ln

echo: echo.c
	gcc -o echo echo.c

pwd: pwd.c
	gcc -o pwd pwd.c

ls: ls.c
	gcc -o ls ls.c

mv: mv.c
	gcc -o mv mv.c

cp: cp.c
	gcc -o cp cp.c

ln: ln.c
	gcc -o ln ln.c

clean:
	rm -f echo pwd ls mv cp ln

.PHONY: all clean
