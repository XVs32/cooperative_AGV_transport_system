g++ -c -o camera.o camera.c ./lib/ZXing/ZXingCore/libZXingCore.a -I /usr/local/include/ -I ./include/ZXing/core/src/ -I ./include/ZXing/thirdparty/stb/ -L/opt/vc/lib/ -lopencv_core -lraspicam -lraspicam_cv -lmmal_core -lmmal_util -lmmal -lopencv_imgproc -lopencv_imgcodecs -lzbar  -lpthread -Wall -Wextra -rdynamic
ar -rcs libcamera.a camera.o

echo "/////////////////END camera///////////////////////"

echo "/////////////////START main///////////////////////"

gcc main.c libcamera.a -lpthread -o main



