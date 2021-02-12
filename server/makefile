
CC=g++
opt=-O2 -c -fpermissive -Wunused-result

ar=ar rcs

header=-I /usr/local/include/ -I ./src/lib/
lib_adr=./src/lib
bin_adr=./bin

all: clean tcp_handler endec cJSON config_manager script_manager main

tcp_handler:
	$(CC) $(opt) $(lib_adr)/tcp_handler.cpp $(header) -o $(bin_adr)/tcp_handler.o
	$(ar) $(bin_adr)/libTcpHandler.a $(bin_adr)/tcp_handler.o
	rm $(bin_adr)/tcp_handler.o

endec:
	$(CC) $(opt) $(lib_adr)/endec.cpp $(header) -o $(bin_adr)/endec.o
	$(ar) $(bin_adr)/libEndec.a $(bin_adr)/endec.o
	rm $(bin_adr)/endec.o

cJSON:
	$(CC) $(opt) $(lib_adr)/cJSON.cpp $(header) -o $(bin_adr)/cJSON.o
	$(ar) $(bin_adr)/libcJSON.a $(bin_adr)/cJSON.o
	rm $(bin_adr)/cJSON.o

config_manager:
	$(CC) $(opt) $(lib_adr)/config_manager.cpp $(header) -o $(bin_adr)/config_manager.o
	$(ar) $(bin_adr)/libConfigManager.a $(bin_adr)/config_manager.o
	rm $(bin_adr)/config_manager.o

script_manager:
	$(CC) $(opt) $(lib_adr)/script_manager.cpp $(header) -o $(bin_adr)/script_manager.o
	$(ar) $(bin_adr)/libScriptManager.a $(bin_adr)/script_manager.o
	rm $(bin_adr)/script_manager.o

main:
	$(CC) -O2 ./src/main.cpp $(header) \
	-L ./bin/ \
	-lTcpHandler -lEndec -lConfigManager -lScriptManager -lcJSON \
	-lpthread \
	-rdynamic \
	-Wall -Wextra \
	-o $(bin_adr)/main

clean:
	rm -r ./bin
	mkdir bin
