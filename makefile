CC = gcc
CFLAGS = -Wall -Wextra -std=c2x -g

PROG = arvoreB
OBJS = main.o arvoreB.o 

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

main.o: main.c arvoreB.h
	$(CC) $(CFLAGS) -c main.c

arvoreB.o: arvoreB.c arvoreB.h
	$(CC) $(CFLAGS) -c arvoreB.c

run: $(PROG)
	./$(PROG)

valgrind: $(PROG)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(PROG)

clean:
	rm -f *.o $(PROG)
