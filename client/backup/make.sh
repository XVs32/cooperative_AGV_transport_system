

echo "/////////////////remove everything in bin///////////////////////"

rm -r ./bin
mkdir bin

echo "/////////////////START sender///////////////////////"

g++ -O3 -D DEBUG -c ./src/lib/sender.cpp \
-I /usr/local/include/ \
-I ./src/include \
-o ./bin/sender.o

ar rcs ./bin/libsender.a ./bin/sender.o

echo "/////////////////END sender///////////////////////"

echo "/////////////////START timerfd///////////////////////"

g++ -O3 -D DEBUG -c ./src/lib/timer.cpp \
-I /usr/local/include/ \
-I ./src/include \
-o ./bin/timer.o

ar rcs ./bin/libsender.a ./bin/timer.o

echo "/////////////////END timerfd///////////////////////"

echo "/////////////////START camera///////////////////////"

g++ -O3 -D DEBUG -c ./src/lib/camera.cpp \
-I /usr/local/include/ \
-I ./src/include \
-I ./src/include/ZXing/core/src \
-I ./src/include/ZXing/thirdparty/stb/ \
-I ./src/lib \
-o ./bin/camera.o

ar rcs ./bin/libcamera.a ./bin/camera.o

echo "/////////////////END camera///////////////////////"

echo "/////////////////START qr code///////////////////////"

g++ -O3 -D DEBUG -c ./src/lib/qr_reader.cpp \
-I /usr/local/include/ \
-I ./src/include \
-I ./src/include/ZXing/core/src \
-I ./src/include/ZXing/thirdparty/stb/ \
-I ./src/lib \
-o ./bin/qr_reader.o

ar rcs ./bin/libqr_reader.a ./bin/qr_reader.o

echo "/////////////////END qr code///////////////////////"

echo "/////////////////START mouse///////////////////////"

g++ -O3 -D DEBUG -c ./src/lib/mouse.cpp \
-I /usr/local/include/ \
-I ./src/include \
-o ./bin/mouse.o

ar rcs ./bin/libmouse.a ./bin/mouse.o

echo "/////////////////END mouse///////////////////////"

echo "/////////////////START sensor_data_encoder_decoder///////////////////////"

g++ -c ./src/lib/sensor_data_encoder_decoder.cpp -lCRC \
-I /usr/local/include/ \
-I ./src/include \
-o ./bin/sensor_data_encoder_decoder.o 
ar -rcs ./bin/libSensorDataEncoderDecoder.a ./bin/sensor_data_encoder_decoder.o

echo "/////////////////END sensor_data_encoder_decoder///////////////////////"


echo "/////////////////START motor///////////////////////"

g++ -c ./src/lib/motor.cpp -lCRC \
-I /usr/local/include/ \
-I ./src/include \
-o ./bin/motor.o 
ar -rcs ./bin/libmotor.a ./bin/motor.o

echo "/////////////////END motor///////////////////////"


echo "/////////////////START main///////////////////////"

g++ -O3 ./src/main.cpp \
-lpthread \
-lmotor -lcamera -lqr_reader -lmouse -lsender \
-lSensorDataEncoderDecoder \
-I ./src/include \
-L ./src/ \
-L /opt/vc/lib/ \
-L ./src/lib/ZXing/ZXingCore \
-L ./bin \
-lZXingCore \
-fpermissive -lopencv_core -lraspicam -lraspicam_cv -lmmal_core -lmmal_util -lmmal \
-lopencv_imgproc -lopencv_imgcodecs -lpthread -Wall -Wextra -rdynamic \
-lwiringPi \
-o ./bin/main



