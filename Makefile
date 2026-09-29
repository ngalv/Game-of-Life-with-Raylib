OBJS_MAIN = gol.o
CC = g++
LINKING_STUFF = -lraylib


gol: main.o board.o gol.o utils.o
	$(CC) -o gol main.o board.o gol.o utils.o $(LINKING_STUFF)

main.o: main.cpp gol.hpp utils.hpp board.hpp $(LINKING_STUFF)
	$(CC) -c main.cpp -o main.o

gol.o: gol.cpp gol.hpp utils.hpp board.hpp $(LINKING_STUFF)
	$(CC) -c gol.cpp -o gol.o

board.o: board.cpp board.hpp utils.hpp $(LINKING_STUFF)
	$(CC) -c board.cpp -o board.o

utils.o: utils.hpp utils.cpp
	$(CC) -c utils.cpp -o utils.o $(LINKING_STUFF)



.PHONY: run
run: gol
	./gol


.PHONY: clean
clean: 
	rm -rf *.o
	rm -rf *.gch
	rm -rf gol
