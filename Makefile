all:
	gcc -Wall -O3 -c -Bshareable xcmd.c -o xcmd.so
	gcc -Wall -O3 example.c xcmd.c -o example

test:

clean:

