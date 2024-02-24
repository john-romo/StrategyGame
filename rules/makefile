#B=bin/
#B=''
B=

BINARIES=$Bmain.o $Bplayer.o $Bboard.o $Bheadings.o $Bpiece.o $Bking.o $Bguard.o \
	$Brifleman.o $Bspecops.o $Bparatrooper.o $Bengineer.o $Bscout.o $Bsearchlight.o

CPPS=main.cpp player.cpp board.cpp headings.cpp piece.cpp king.cpp guard.cpp \
	rifleman.cpp specops.cpp paratrooper.cpp engineer.cpp scout.cpp searchlight.cpp



.PHONY: default move build back

default: move build back


move:
	mv bin/*.o .

build: $(BINARIES)
	g++ -o run $(BINARIES) 

back:
	mv *.o bin



$Bmain.o: main.cpp main.h board.h defaults.h pieceincludes.h player.h
	g++ -c main.cpp

$Bplayer.o: player.h defaults.h	
	g++ -c player.cpp

$Bboard.o: board.cpp board.h pieceincludes.h defaults.h
	g++ -c board.cpp

$Bheadings.o: headings.cpp headings.h defaults.h
	g++ -c headings.cpp	

$Bpiece.o: piece.cpp piece.h board.h pieceincludes.h defaults.h 
	g++ -c piece.cpp

$Bking.o: king.cpp king.h defaults.h 
	g++ -c king.cpp

$Bguard.o: guard.cpp guard.h defaults.h
	g++ -c guard.cpp

$Brifleman.o: rifleman.cpp rifleman.h defaults.h
	g++ -c rifleman.cpp

$Bspecops.o: specops.cpp specops.h defaults.h
	g++ -c specops.cpp

$Bparatrooper.o: paratrooper.cpp paratrooper.h defaults.h
	g++ -c paratrooper.cpp

$Bengineer.o: engineer.cpp engineer.h defaults.h
	g++ -c engineer.cpp

$Bscout.o: scout.cpp scout.h defaults.h
	g++ -c scout.cpp

$Bsearchlight.o: searchlight.cpp searchlight.h defaults.h
	g++ -c searchlight.cpp

board.h: defaults.h headings.h piece.h
defaults.h: definitions.h
definitions.h:
engineer.h: defaults.h piece.h board.h
guard.h: defaults.h piece.h board.h
headings.h: defaults.h
king.h: defaults.h piece.h board.h
main.h: 
paratrooper.h: defaults.h piece.h board.h
rifleman.h: defaults.h piece.h board.h
scout.h: defaults.h piece.h board.h
specops.h: defaults.h piece.h board.h
searchlight.h: defaults.h piece.h board.h
piece.h: defaults.h headings.h player.h
pieceincludes.h: king.h guard.h rifleman.h specops.h paratrooper.h engineer.h scout.h searchlight.h
player.h: defaults.h


slow:
	g++ -o run $(CPPS)

clean:
	rm *.o












