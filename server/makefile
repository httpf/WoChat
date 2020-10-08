all:server clean

server:server.o
	g++ -g -o server server.o

server.o:server.cpp
	g++ -g -c server.cpp

clean:
	rm server.o
