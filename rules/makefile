
BINARIES=main.o player.o board.o headings.o piece.o test.o

CPPS=main.cpp player.cpp board.cpp headings.cpp piece.cpp test.cpp


.PHONY: default move build back slow clean

default: move build back

move:
	mv bin/*.o .

build: $(BINARIES)
	g++ -o run $(BINARIES) 

back:
	mv *.o bin



main.o: main.cpp main.h board.h defaults.h player.h test.h definitions.h
	g++ -c main.cpp

player.o: player.h defaults.h definitions.h
	g++ -c player.cpp

board.o: board.cpp board.h defaults.h definitions.h
	g++ -c board.cpp

headings.o: headings.cpp headings.h defaults.h definitions.h
	g++ -c headings.cpp	

piece.o: piece.cpp piece.h board.h defaults.h definitions.h 
	g++ -c piece.cpp

test.o: test.cpp board.h piece.h headings.h player.h definitions.h
	g++ -c test.cpp


board.h: defaults.h headings.h piece.h
defaults.h: definitions.h
definitions.h:
engineer.h: defaults.h piece.h board.h
guard.h: defaults.h piece.h board.h
headings.h: defaults.h
main.h: 
piece.h: defaults.h headings.h player.h
player.h: defaults.h
test.h: defaults.h definitions.h board.h piece.h headings.h player.h

slow:
	g++ -o run $(CPPS)

clean:
	rm *.o












