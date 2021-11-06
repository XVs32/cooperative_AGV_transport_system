
CC=g++
opt=-O2 -c

ar=ar rcs

header=-I /usr/local/include/ -I ./src/lib/
lib_adr=./src/lib
bin_adr=./bin


ifdef DEBUG
	opt=-O2 -D DEBUG -c
endif

all: clean log ipc_handler tcp_handler timerfd camera qr_code qe endec motor command_manager command main

log:
	$(CC) $(opt) $(lib_adr)/log.c $(header) -o $(bin_adr)/log.o
	$(ar) $(bin_adr)/libLog.a $(bin_adr)/log.o

ipc_handler:
	$(CC) $(opt) $(lib_adr)/ipc_handler.c $(header) -o $(bin_adr)/ipc_handler.o
	$(ar) $(bin_adr)/libIpcHandler.a $(bin_adr)/ipc_handler.o

tcp_handler:
	$(CC) $(opt) $(lib_adr)/tcp_handler.c $(header) -o $(bin_adr)/tcp_handler.o
	$(ar) $(bin_adr)/libTcpHandler.a $(bin_adr)/tcp_handler.o

timerfd:
	$(CC) $(opt) $(lib_adr)/timer.c $(header) -o $(bin_adr)/timer.o
	$(ar) $(bin_adr)/libTimer.a $(bin_adr)/timer.o

camera:
	$(CC) $(opt) $(lib_adr)/camera.c $(header) -o $(bin_adr)/camera.o
	$(ar) $(bin_adr)/libCamera.a $(bin_adr)/camera.o

qr_code:
	$(CC) $(opt) $(lib_adr)/qr_reader.c $(header) \
	-I ./src/lib/quirc/demo \
	-I ./src/lib/quirc/lib \
	-I /usr/include/SDL \
	-o $(bin_adr)/qr_reader.o
	$(ar) $(bin_adr)/libQrReader.a $(bin_adr)/qr_reader.o

qe:
	$(CC) $(opt) $(lib_adr)/qe.c $(header) -o $(bin_adr)/qe.o
	$(ar) $(bin_adr)/libQe.a $(bin_adr)/qe.o

endec:
	$(CC) $(opt) $(lib_adr)/endec.c $(header) -o $(bin_adr)/endec.o
	$(ar) $(bin_adr)/libEndec.a $(bin_adr)/endec.o
	
command_manager:
	$(CC) $(opt) $(lib_adr)/command_manager.c $(header) -o $(bin_adr)/command_manager.o
	$(ar) $(bin_adr)/libCommandManager.a $(bin_adr)/command_manager.o

motor:
	$(CC) $(opt) $(lib_adr)/motor.c $(header) -o $(bin_adr)/motor.o
	$(ar) $(bin_adr)/libMotor.a $(bin_adr)/motor.o
	
command:
	$(CC) $(opt) $(lib_adr)/command.c $(header) \
	-I ./src/include/ZXing/core/src \
	-I ./src/include/ZXing/thirdparty/stb/ \
	-o $(bin_adr)/command.o
	$(ar) $(bin_adr)/libCommand.a $(bin_adr)/command.o

main:
	$(CC) -O3 ./src/main.c $(header) \
	-L ./bin/ \
	-L /opt/vc/lib/ \
	-lCommandManager -lMotor -lCamera -lQrReader -lQe -lTcpHandler -lEndec -lTimer -lIpcHandler -lquirc -lCommand -lLog \
	-lopencv_core -lraspicam -lraspicam_cv -lmmal_core -lmmal_util -lmmal \
	-lopencv_imgproc -lopencv_imgcodecs -lpthread -lwiringPi -lm \
	-rdynamic \
	-Wall -Wextra \
	-o $(bin_adr)/main

clean:
	rm -r ./bin
	mkdir bin 
	cp ./bak/libquirc.a ./bin/libquirc.a
	rm -f raspicam_image_*
