LOPTS = -ansi -std=c11

all: connectFour

connectFour: connectFour.c
	gcc $(LOPTS) -o connectFour.exe connectFour.c

clean:
	rm connectFour.exe


