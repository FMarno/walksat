COMMAND = clang -Wall -Wextra -g -Ofast
all: WalkSAT

clean:
	rm *.o WalkSAT

WalkSAT:	main.o parser.o walksat.o list.o
	$(COMMAND) main.o parser.o walksat.o list.o -o WalkSAT

main.o: main.c
	$(COMMAND) main.c -c -o main.o

parser.o:	parser.c walksat.h
	$(COMMAND) parser.c -c -o parser.o

walksat.o:	walksat.c walksat.h
	$(COMMAND) walksat.c -c -o walksat.o

list.o: list.c list.h
	$(COMMAND) list.c -c -o list.o
