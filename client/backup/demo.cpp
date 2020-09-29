////////////////////////////////////////////////////////////////////////////
//
//  This file is part of RTIMULib
//
//  Copyright (c) 2014-2015, richards-tech, LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in
//  the Software without restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
//  Software, and to permit persons to whom the Software is furnished to do so,
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include <math.h>
#include "RTIMULib.h"
#include <wiringPi.h>

#include <ctime>
#include <raspicam/raspicam_cv.h>
//#include "raspicam.h"
//#include "raspicam_cv.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>
#include <iostream>

using namespace cv;
using namespace std;
using namespace zbar;



#define LEFT -1
#define RIGHT 1

RTIMU *imu;
raspicam::RaspiCam_Cv Camera;
cv::Mat image;
ImageScanner scanner;  

void pin_init(){
    wiringPiSetup () ;
    pinMode (0, OUTPUT) ;
    pinMode (1, OUTPUT) ;
    pinMode (3, OUTPUT) ;
    pinMode (4, OUTPUT) ;
}

void RTIMU_init(){

    RTIMUSettings *settings = new RTIMUSettings("RTIMULib");

    imu = RTIMU::createIMU(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        printf("No IMU found\n");
        exit(1);
    }

    imu->IMUInit();

    imu->setSlerpPower(0.02);
    imu->setGyroEnable(true);
    imu->setAccelEnable(true);
    imu->setCompassEnable(true);
    

}

float angle_diff(float a, float b){
    
    float ans = abs(a - b);
    
    if(abs(ans)>180){
        ans = abs(abs(ans)-360);
    }
    return ans;
}



void motor_stop(){
    digitalWrite (0, LOW) ;
    digitalWrite (1, LOW) ;
    digitalWrite (3, LOW) ;
    digitalWrite (4, LOW) ;
};

void motor_right(){
    digitalWrite (0, HIGH) ;
    digitalWrite (1, LOW) ;
    digitalWrite (3, HIGH) ;
    digitalWrite (4, LOW) ;
};

void motor_left(){
    digitalWrite (0, LOW) ;
    digitalWrite (1, HIGH) ;
    digitalWrite (3, LOW) ;
    digitalWrite (4, HIGH) ;
};

void motor_forward(){
    digitalWrite (0, HIGH) ;
    digitalWrite (1, LOW) ;
    digitalWrite (3, LOW) ;
    digitalWrite (4, HIGH) ;
}

void motor_backward(){
    digitalWrite (0, LOW);
    digitalWrite (1, HIGH) ;
    digitalWrite (3, HIGH) ;
    digitalWrite (4, LOW) ;
}


void turn_with_angle(int way, float angle){
    
    int sampleCount = 0;
    int sampleRate = 0;
    uint64_t rateTimer;
    uint64_t displayTimer;
    uint64_t now;
    
    rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();
    
    RTIMU_DATA imuData = imu->getIMUData();
    float angle_start = imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE;
    
    printf("\nangle_start %f\n",angle_start);

    if(way<0){
	motor_left();
    }
    else{
	motor_right();
    }


    while(1) {
        imu->IMURead();
        //  poll at the rate recommended by the IMU
        usleep(imu->IMUGetPollInterval() * 1000);
        imuData = imu->getIMUData();
		
        sampleCount++;

        now = RTMath::currentUSecsSinceEpoch();

	    float angle_now = imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE; 
            
        if(angle_diff(angle_start,angle_now)>angle){
            printf("\nangle_end %f\n",angle_now);
	        motor_stop();
            break;
            
        }

        //  display 10 times per second
        if ((now - displayTimer) > 100000) {
					
            printf("Sample rate %d: yaw %f diff %f\r", \
                    sampleRate, imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE, \
                    angle_diff(angle_start,imuData.fusionPose.z() * RTMATH_RAD_TO_DEGREE));
            fflush(stdout);
            displayTimer = now;          
        }

        //  update rate every second
        if ((now - rateTimer) > 1000000) {
            sampleRate = sampleCount;
            sampleCount = 0;
            rateTimer = now;
        }
            
    }

}

void cam_init(){
	
    //scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);  
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);
    // enable qr
    scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
	//set camera params
	Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
	cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;}
	
}

float get_qr_angle(){
	
	Camera.grab();
    Camera.retrieve (image);
	
    // extract results  
       
    int width = image.cols;  
	int height = image.rows;  

	uchar *raw = (uchar *)image.data;  
	// wrap image data  
	Image scan_image(width, height, "Y800", raw, width * height);  

	int n = scanner.scan(scan_image);  
	// extract results  
	for(Image::SymbolIterator symbol = scan_image.symbol_begin();  
	symbol != scan_image.symbol_end();  
	++symbol) {  
	   vector<Point> vp;  
	   // do something useful with results  
	   cout << "decoded " << symbol->get_type_name()  << " symbol \"" << symbol->get_data() << '"' <<" "<< endl;  
	   int n = symbol->get_location_size();  
	   for(int i=0;i<n;i++){  
		   vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i))); 
		   printf("%d %d\n", symbol->get_location_x(i),symbol->get_location_y(i));
	   }  
	   RotatedRect r = minAreaRect(vp);  
	   cout<<"Angle: "<<r.angle<<endl; 
		return r.angle; 
	}  
	return -1024; 
}

void angle_correction(){

	while(1){
		float angle = get_qr_angle();
		if(angle>-70){
			motor_left();
		}
		else if(angle<-71){
			motor_right();
		}
		else{
			break;
		}
	
	}
	
	return;
}




int main()
{
    
    
    

    //  Using RTIMULib here allows it to use the .ini file generated by RTIMULibDemo.
    //  Or, you can create the .ini in some other directory by using:
    //      RTIMUSettings *settings = new RTIMUSettings("<directory path>", "RTIMULib");
    //  where <directory path> is the path to where the .ini file is to be loaded/saved

    
    pin_init();

    RTIMU_init();
	
	cam_init();

    //  set up for rate timer

    
    

    /*motor_forward();
    sleep(1);

    turn_with_angle(1, 90);
    sleep(1);

    motor_forward();
    sleep(1);

    turn_with_angle(1,90); */
	
	angle_correction();
	
	motor_stop();
    
	Camera.release();
    return 0;
}







