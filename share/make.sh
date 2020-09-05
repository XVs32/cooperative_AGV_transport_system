echo "/////////////////START sensor_data_encoder_decoder///////////////////////"

g++ -c -o ./exec/sensor_data_encoder_decoder.o ./src/sensor_data_encoder_decoder.cpp -lCRC
ar -rcs ./exec/libSensorDataEncoderDecoder.a ./exec/sensor_data_encoder_decoder.o


