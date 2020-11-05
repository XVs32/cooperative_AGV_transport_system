
CC=g++
opt=-O3 -c

ar=ar rcs

header=-I /usr/local/include/ -I ./src/include/
lib_adr=./src/lib
bin_adr=./bin



all: clean tcp_handler endec main

tcp_handler:
	$(CC) $(opt) $(lib_adr)/tcp_handler.cpp $(header) -o $(bin_adr)/tcp_handler.o
	$(ar) $(bin_adr)/libTcpHandler.a $(bin_adr)/tcp_handler.o
	
endec:
	$(CC) $(opt) $(lib_adr)/endec.cpp $(header) -o $(bin_adr)/endec.o
	$(ar) $(bin_adr)/libEndec.a $(bin_adr)/endec.o

main:
	$(CC) -O3 ./src/main.cpp $(header) \
	-L ./bin/ \
	-lTcpHandler -lEndec -lpthread \
	-rdynamic \
	-Wall -Wextra \
	-o $(bin_adr)/main

clean:
	rm -r ./bin
	mkdir bin