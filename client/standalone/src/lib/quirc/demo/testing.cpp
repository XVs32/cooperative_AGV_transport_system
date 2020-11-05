#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <cstdio>

#include <quirc.h>

using namespace cv;

int main(){
	
	Mat image = imread("raspicam_image_7.jpg", IMREAD_GRAYSCALE);
	
	struct quirc *qr;
	qr = quirc_new();
	if (!qr) {
		perror("Failed to allocate memory");
		abort();
	}
	
	if (quirc_resize(qr, 320, 240) < 0) {
		perror("Failed to allocate video memory");
		abort();
	}
	
	
	uint8_t *ptr;
	int w, h;

	ptr = quirc_begin(qr, &w, &h);
	
	memcpy(ptr,image.data,sizeof(uint8_t)*320*240);
	
	/* Fill out the image buffer here.
	 * image is a pointer to a w*h bytes.
	 * One byte per pixel, w pixels per line, h lines in the buffer.
	 */
	
	quirc_end(qr);
	
	
	image.data
	quirc_destroy(qr);
	return 0;
	
}

