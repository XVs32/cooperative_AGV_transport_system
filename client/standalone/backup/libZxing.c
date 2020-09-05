/*
* Copyright 2016 Nu-book Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "ReadBarcode.h"
#include "TextUtfEncoding.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace ZXing;



std::ostream& operator<<(std::ostream& os, const std::vector<ResultPoint>& points) {
	for (const auto& p : points)
		os << int(p.x() + .5f) << "x" << int(p.y() + .5f) << " ";
	return os;
}



 
int main ( int argc,char **argv ) {
	
	
	bool fastMode = true;
	bool tryRotate = true;
		
	std::string format = "QR_CODE";
	
	
   
    for ( int i=0; i<nCount; i++ ) {
        Camera.grab();
        Camera.retrieve (image);
	
        // extract results  
       
        int width = image.cols;  
        int height = image.rows;  
        
        uchar *raw = (uchar *)image.data;  
		
		printf("image: %d %d %d \n",raw[0],raw[1],raw[2]);
		
		auto result = ReadBarcode(width, height, raw, width , 1, 0, 1, 2, {BarcodeFormatFromString(format)}, tryRotate, !fastMode);

		if (result.isValid()) {
			std::cout << "Text:     " << TextUtfEncoding::ToUtf8(result.text()) << "\n"
					  << "Format:   " << ToString(result.format()) << "\n"
					  << "Position: " << result.resultPoints() << "\n";
			auto errLevel = result.metadata().getString(ResultMetadata::Key::ERROR_CORRECTION_LEVEL);
			if (!errLevel.empty()) {
				std::cout << "EC Level: " << TextUtfEncoding::ToUtf8(errLevel) << "\n";
			}
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

