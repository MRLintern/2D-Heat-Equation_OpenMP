CC = gcc
CFLAGS = -g -O2 -Wall -Wextra -fopenmp


all: 2D_heat
	
2D_heat: main.c
	$(CC) $(CFLAGS) -o 2D_heat main.c -lm
	
clean:
	2D_heat

