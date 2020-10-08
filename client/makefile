all:client clean

client:client.o
	g++ -g -o client client.o

client.o:client.cpp
	g++ -g -c client.cpp

clean:
	rm client.o