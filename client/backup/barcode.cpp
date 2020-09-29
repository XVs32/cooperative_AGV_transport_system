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
 
int main ( int argc,char **argv ) {
	
	ImageScanner scanner;  
    //scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);  
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);
    // enable qr
    scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
	
	
   
    time_t timer_begin,timer_end;
    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;
    int nCount=100;
    //set camera params
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    //Start capture
    cout<<"Capturing "<<nCount<<" frames ...."<<endl;
    time ( &timer_begin );
    for ( int i=0; i<nCount; i++ ) {
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
        }  


	
		
        
    }
    cout<<"Stop camera..."<<endl;
    Camera.release();
    //show time statistics
    time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
    double secondsElapsed = difftime ( timer_end,timer_begin );
    cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<<  ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
    //save image 
    cv::imwrite("raspicam_cv_image.jpg",image);
    cout<<"Image saved at raspicam_cv_image.jpg"<<endl;
}
