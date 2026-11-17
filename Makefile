CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: jeu

jeu: sae1-2.o board.o
	$(CC) $(CFLAGS) -o jeu sae1-2.o board.o

%.o: %.c board.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o jeu
