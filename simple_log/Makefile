CC = cc

ROOTPATH=../../..
INCLUDE = -I./

CFLAGS = -O0 -g3 -Wall
target = main

test: main.o logger.o
		gcc -o test $^
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	mv $@ out
clean:
	rm -rf out/*
