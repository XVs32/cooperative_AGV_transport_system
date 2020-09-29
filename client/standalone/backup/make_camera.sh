#g++ -o camera -D DEBUG ./lib/camera.cpp ./lib/ZXing/ZXingCore/libZXingCore.a \
#-I /usr/local/include/ \
#-I ./include \
#-I ./include/ZXing/core/src \
#-I ./include/ZXing/thirdparty/stb/ \
#-I ./lib \
#-L/opt/vc/lib/ \
#-fpermissive -lopencv_core -lraspicam -lraspicam_cv -lmmal_core -lmmal_util -lmmal -lopencv_imgproc -lopencv_imgcodecs -lpthread -Wall -Wextra -rdynamic


g++ -D DEBUG ./lib/camera.cpp \
-I /usr/local/include/ \
-I ./include \
-I ./include/ZXing/core/src \
-I ./include/ZXing/thirdparty/stb/ \
-I ./lib \
-L /opt/vc/lib/ \
-L ./lib/ZXing/ZXingCore \
-lZXingCore \
-fpermissive -lopencv_core -lraspicam -lraspicam_cv -lmmal_core -lmmal_util -lmmal \
-lopencv_imgproc -lopencv_imgcodecs -lpthread -Wall -Wextra -rdynamic \
-o camera.o