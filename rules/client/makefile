HDIR=../

BINARIES=client.o player.o board.o heading.o piece.o test.o task.o
CPPS=client.cpp player.cpp board.cpp heading.cpp piece.cpp test.cpp task.cpp

.PHONY: default move build back slow clean

default: move build back

move:
	mv bin/*.o .

build: $(BINARIES)
	g++ -o run $(BINARIES) 

back:
	mv *.o bin

slow:
	g++ -o run $(CPPS)

clean:
	rm *.o


client.o: client.cpp client.h board.h player.h task.h heading.h test.h $(HDIR)default.h $(HDIR)coms.h
	g++ -c client.cpp
player.o: player.cpp player.h
	g++ -c player.cpp
board.o: board.cpp board.h 
	g++ -c board.cpp
heading.o: heading.cpp heading.h 
	g++ -c heading.cpp	
piece.o: piece.cpp piece.h board.h  
	g++ -c piece.cpp
task.o: task.cpp task.h 
	g++ -c task.cpp
test.o: test.cpp test.h 
	g++ -c test.cpp

client.h: $(HDIR)default.h $(HDIR)coms.h board.h player.h task.h test.h heading.h
player.h: $(HDIR)default.h
board.h: $(HDIR)default.h heading.h piece.h
headings.h: $(HDIR)default.h
piece.h: $(HDIR)default.h heading.h player.h 
task.h: $(HDIR)default.h board.h piece.h
test.h: $(HDIR)default.h board.h piece.h heading.h player.h task.h
defaults.h: 
