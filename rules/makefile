comp: move main.o board.o headings.o piece.o king.o guard.o \
	rifleman.o specops.o paratrooper.o engineer.o scout.o \
	searchlight.o player.o
	g++ -o run main.o board.o headings.o piece.o king.o guard.o \
	rifleman.o specops.o paratrooper.o engineer.o scout.o searchlight.o player.o
	mv *.o bin 

move:
	./move

.PHONY: move comp

main.o: main.cpp main.h board.h defaults.h pieceincludes.h player.h
	g++ -c main.cpp

board.o: board.cpp board.h pieceincludes.h defaults.h
	g++ -c board.cpp

headings.o: headings.cpp headings.h defaults.h
	g++ -c headings.cpp	

piece.o: piece.cpp piece.h board.h pieceincludes.h defaults.h 
	g++ -c piece.cpp

king.o: king.cpp king.h defaults.h 
	g++ -c king.cpp

guard.o: guard.cpp guard.h defaults.h
	g++ -c guard.cpp

rifleman.o: rifleman.cpp rifleman.h defaults.h
	g++ -c rifleman.cpp

specops.o: specops.cpp specops.h defaults.h
	g++ -c specops.cpp

paratrooper.o: paratrooper.cpp paratrooper.h defaults.h
	g++ -c paratrooper.cpp

engineer.o: engineer.cpp engineer.h defaults.h
	g++ -c engineer.cpp

scout.o: scout.cpp scout.h defaults.h
	g++ -c scout.cpp

searchlight.o: searchlight.cpp searchlight.h defaults.h
	g++ -c searchlight.cpp

player.o: player.h defaults.h	
	g++ -c player.cpp

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
	g++ -o run main.cpp board.cpp headings.cpp piece.cpp king.cpp guard.cpp \
	rifleman.cpp specops.cpp paratrooper.cpp engineer.cpp scout.cpp searchlight.cpp player.cpp


clean:
	rm *.o












