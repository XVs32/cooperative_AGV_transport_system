
CC=g++
opt=-O3 -c

ar=ar rcs

header=-I /usr/local/include/ -I ./src/include/
lib_adr=./src/lib
bin_adr=./bin



all: clean sender timerfd camera qr_code mouse sensor_data_encoder_decoder motor main

sender:
	$(CC) $(opt) $(lib_adr)/sender.cpp $(header) -o $(bin_adr)/sender.o
	$(ar) $(bin_adr)/libSender.a $(bin_adr)/sender.o

timerfd:
	$(CC) $(opt) $(lib_adr)/timer.cpp $(header) -o $(bin_adr)/timer.o
	$(ar) $(bin_adr)/libTimer.a $(bin_adr)/timer.o

camera:
	$(CC) $(opt) $(lib_adr)/camera.cpp $(header) -o $(bin_adr)/camera.o
	$(ar) $(bin_adr)/libCamera.a $(bin_adr)/camera.o

qr_code:
	$(CC) $(opt) $(lib_adr)/qr_reader.cpp $(header) \
	-I ./src/include/ZXing/core/src \
	-I ./src/include/ZXing/thirdparty/stb/ \
	-o $(bin_adr)/qr_reader.o
	$(ar) $(bin_adr)/libQrReader.a $(bin_adr)/qr_reader.o

mouse:
	$(CC) $(opt) $(lib_adr)/mouse.cpp $(header) -o $(bin_adr)/mouse.o
	$(ar) $(bin_adr)/libMouse.a $(bin_adr)/mouse.o

sensor_data_encoder_decoder:
	$(CC) $(opt) $(lib_adr)/sensor_data_encoder_decoder.cpp $(header) -o $(bin_adr)/sensor_data_encoder_decoder.o
	$(ar) $(bin_adr)/libSensorDataEncoderDecoder.a $(bin_adr)/sensor_data_encoder_decoder.o

motor:
	$(CC) $(opt) $(lib_adr)/motor.cpp $(header) -o $(bin_adr)/motor.o
	$(ar) $(bin_adr)/libMotor.a $(bin_adr)/motor.o

main:
	$(CC) -O3 ./src/main.cpp $(header) \
	-L ./bin/ \
	-L /opt/vc/lib/ \
	-L ./src/lib/ZXing/ZXingCore \
	-lMotor -lCamera -lQrReader -lMouse -lSender -lSensorDataEncoderDecoder -lTimer \
	-lZXingCore -lopencv_core -lraspicam -lraspicam_cv -lmmal_core -lmmal_util -lmmal \
	-lopencv_imgproc -lopencv_imgcodecs -lpthread -lwiringPi \
	-rdynamic \
	-Wall -Wextra \
	-o $(bin_adr)/main

clean:
	rm -r ./bin
	mkdir bin