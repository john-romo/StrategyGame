
# BINARIES=main.o player.o board.o heading.o piece.o test.o
# CPPS=main.cpp player.cpp board.cpp heading.cpp piece.cpp test.cpp

# .PHONY: default move build back slow clean

# default: move build back

# build: $(BINARIES)
# 	g++ -o run $(BINARIES) 

# slow:
# 	g++ -o run $(CPPS)

# clean:
# 	rm *.o


# main.o: main.cpp main.h board.h player.h test.h default.h 
# 	g++ -c main.cpp
# player.o: player.cpp player.h
# 	g++ -c player.cpp
# board.o: board.cpp board.h 
# 	g++ -c board.cpp
# heading.o: heading.cpp heading.h 
# 	g++ -c heading.cpp	
# piece.o: piece.cpp piece.h board.h  
# 	g++ -c piece.cpp
# test.o: test.cpp test.h 
# 	g++ -c test.cpp

# main.h: 
# player.h: default.h
# board.h: default.h heading.h piece.h
# headings.h: default.h
# piece.h: default.h heading.h player.h
# test.h: default.h board.h piece.h heading.h player.h
# defaults.h: 

tbsgame: main.cpp
	g++ main.cpp button2.cpp picture.cpp tile.cpp camera.cpp piece.cpp board.cpp heading.cpp player.cpp -lSDL2 -lSDL2_image -o tbsgame.out

















