echo "/////////////////START TCP_server///////////////////////"

g++ -c -o ./exec/TCP_server.o ./src/TCP_server.cpp -lCRC
ar -rcs ./exec/libTCPServer.a ./exec/TCP_server.o

echo "/////////////////START main///////////////////////"

g++ -o ./exec/main ./src/main.cpp ./exec/libTCPServer.a -I ../share/src ../share/exec/libSensorDataEncoderDecoder.a -lpthread 


