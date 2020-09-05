#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>
#include <cctype>

#include "camera.h"

using namespace ZXing;
using namespace cv;
using namespace std;

 
int main ( int argc,char **argv ) {
	
	
	
	pthread_t t_qr_reader; 
	
	pthread_create(&t_qr_reader, NULL, qr_reader, NULL);
	
	pthread_t t_qr_reader; 
	
	pthread_create(&t_qr_reader, NULL, qr_reader, NULL);
	
	camera();
	
	pthread_join(t_qr_reader, NULL); 
	//pthread_create(&t_qr_reader, NULL, qr_reader, NULL);
	
	//save image 
	cv::imwrite("raspicam_cv_image.jpg",image_B);
	cout<<"Image saved at raspicam_cv_image.jpg"<<endl;
	
	return 0;
	
}



