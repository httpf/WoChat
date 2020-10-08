all:client clean

client:client.o main.o
	g++ -g -o client client.o main.o

main.o:main.cpp
	g++ -g -c main.cpp

client.o:client.cpp
	g++ -g -c client.cpp

clean:
	rm client.o main.o