g++ = g++ -std=c++14

minesweeper.exe : main.o title.o game.o minesweeper.o
	$(g++) -o minesweeper.exe main.o title.o game.o minesweeper.o -lncurses

main.o : main.cpp
	$(g++) -c main.cpp

title.o : title.cpp
	$(g++) -c title.cpp

game.o : game.cpp
	$(g++) -c game.cpp

minesweeper.o : minesweeper.cpp
	$(g++) -c minesweeper.cpp

clear :
	rm *.o
	rm minesweeper.exe