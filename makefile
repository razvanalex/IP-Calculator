CC = gcc
FLAGS = -Wall -lm
SRC = tema_ip.c
EXE = ip

build: $(SRC)
	$(CC) $(SRC) -o $(EXE) $(FLAGS)

run:
	./$(EXE)

.PHONY: clean
clean:
	rm -f *.o $(EXE)
