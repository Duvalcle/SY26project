#include "opencv2/opencv.hpp"
#include <raspicam/raspicam.h>
#include <raspicam/raspicam_cv.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;
using namespace cv;


int main(int narg, char *argv[]) {
	
    raspicam::RaspiCam_Cv Camera;
    Camera.set ( CV_CAP_PROP_FORMAT, CV_32FC3);
    Camera.set ( CV_CAP_PROP_FRAME_WIDTH,  640 );
    Camera.set ( CV_CAP_PROP_FRAME_HEIGHT, 480);
    Camera.set ( CV_CAP_PROP_BRIGHTNESS,60 );
    Camera.set ( CV_CAP_PROP_CONTRAST ,65);
    Camera.set ( CV_CAP_PROP_SATURATION, 60);
    Camera.set ( CV_CAP_PROP_GAIN, 0 );
    Camera.set ( CV_CAP_PROP_EXPOSURE, 50 );   
    if (!Camera.open()) {
		  cerr<<"Error opening the camera"<<endl;
		  return -1;
	}
	
	int kernel_size = 6;
	double sig =1.5, lm = 3, gm = 0.04, ps = 0;
	
	double theta[4];
	//theta[0] = 180;
	//theta[1] = 225;
	//theta[2] = 270;
	//theta[3] = 135;
	
	theta[0] = 0 ;
	theta[1] = CV_PI/4 ;
	theta[2] = CV_PI/2 ;
	theta[3] = 3 * CV_PI/4 ;
	
    Mat frame;
   
    cout << "on filme" << endl;
    
    Mat kernel[4];
    for(int i =0; i<4; i++){
		kernel[i] = getGaborKernel(Size(kernel_size,kernel_size), sig, theta[i], lm,gm, ps, CV_32F);
		//cout << kernel[i] << endl;
	}
    
    for(;;) {
		Camera.grab();
		//Mat sobx_2, soby_2, sobx, soby, result;
		Mat temp[4];
		Mat result;
        Camera.retrieve ( frame);
        cvtColor(frame,frame,COLOR_BGR2GRAY); //oui ou non ???
        //Sobel(frame, sobx, CV_8U, 1, 0);
        //Sobel(frame, soby, CV_8U, 0, 1);
        //pow(sobx, 2.0, sobx_2);
        //pow(soby_2, 2.0, soby_2);
        //sqrt(sobx + sobx_2, result);
        //result = sobx + soby;
        
        for(int i =0; i<4; i++){
			filter2D(frame, temp[i], CV_32F, kernel[i]);
			//cout << temp[i] << endl;
			bitwise_not(temp[i], temp[i]);
		}
        
		//imshow("camera0", temp[0]);
		//imshow("camera1", temp[1]);
		//imshow("camera2", temp[2]);
		//imshow("camera3", temp[3]);
		
		Mat temp_2[4];
		
		 for(int i =0; i<4; i++){
			pow(temp[i], 2.0,  temp_2[i]);
		}
		
		sqrt(temp_2[0] +temp_2[1] + temp_2[2] + temp_2[3], result);
		
		imshow("camera", result);
		
        if(waitKey(10) >= 0) break;
    }
    Camera.release();
    cout << "that's all folk" << endl;
    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
