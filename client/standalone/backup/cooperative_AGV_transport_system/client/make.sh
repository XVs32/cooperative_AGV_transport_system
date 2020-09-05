echo "/////////////////START TCP_client///////////////////////"

g++ -o ./exec/TCP_client  ./src/TCP_client.c -I ../share/src ../share/exec/libSensorDataEncoderDecoder.a -lpthread 