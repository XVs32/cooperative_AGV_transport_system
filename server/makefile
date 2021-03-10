
CC=gcc
opt=-O2 -c

ar=ar rcs

header=-I /usr/local/include/ -I ./src/lib/
lib_adr=./src/lib
bin_adr=./bin

all: clean log ipc_handler tcp_handler endec cJSON config_manager script_manager linklist main

log:
	$(CC) $(opt) $(lib_adr)/log.c $(header) -o $(bin_adr)/log.o
	$(ar) $(bin_adr)/libLog.a $(bin_adr)/log.o
	rm $(bin_adr)/log.o

ipc_handler:
	$(CC) $(opt) $(lib_adr)/ipc_handler.c $(header) -o $(bin_adr)/ipc_handler.o
	$(ar) $(bin_adr)/libIpcHandler.a $(bin_adr)/ipc_handler.o
	rm $(bin_adr)/ipc_handler.o

tcp_handler:
	$(CC) $(opt) $(lib_adr)/tcp_handler.c $(header) -o $(bin_adr)/tcp_handler.o
	$(ar) $(bin_adr)/libTcpHandler.a $(bin_adr)/tcp_handler.o
	rm $(bin_adr)/tcp_handler.o

endec:
	$(CC) $(opt) $(lib_adr)/endec.c $(header) -o $(bin_adr)/endec.o
	$(ar) $(bin_adr)/libEndec.a $(bin_adr)/endec.o
	rm $(bin_adr)/endec.o

cJSON:
	$(CC) $(opt) $(lib_adr)/cJSON.c $(header) -o $(bin_adr)/cJSON.o
	$(ar) $(bin_adr)/libcJSON.a $(bin_adr)/cJSON.o
	rm $(bin_adr)/cJSON.o

config_manager:
	$(CC) $(opt) $(lib_adr)/config_manager.c $(header) -o $(bin_adr)/config_manager.o
	$(ar) $(bin_adr)/libConfigManager.a $(bin_adr)/config_manager.o
	rm $(bin_adr)/config_manager.o

script_manager:
	$(CC) $(opt) $(lib_adr)/script_manager.c $(header) -o $(bin_adr)/script_manager.o
	$(ar) $(bin_adr)/libScriptManager.a $(bin_adr)/script_manager.o
	rm $(bin_adr)/script_manager.o

linklist:
	$(CC) $(opt) $(lib_adr)/linklist.c $(header) -o $(bin_adr)/linklist.o
	$(ar) $(bin_adr)/libLinklist.a $(bin_adr)/linklist.o
	rm $(bin_adr)/linklist.o

main:
	$(CC) -O2 ./src/main.c $(header) \
	-L ./bin/ \
	-lTcpHandler -lEndec -lScriptManager -lConfigManager -lIpcHandler  -lLinklist -lLog -lcJSON \
	-lpthread -lm \
	-rdynamic \
	-Wall -Wextra \
	-o $(bin_adr)/main

clean:
	rm -r ./bin
	mkdir bin
