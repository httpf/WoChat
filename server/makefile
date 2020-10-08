all:server clean

server:server.o main.o
	g++ -g -o server main.o server.o

main.o:main.cpp
	g++ -g -c main.cpp

server.o:server.cpp
	g++ -g -c server.cpp

clean:
	rm server.o main.o
