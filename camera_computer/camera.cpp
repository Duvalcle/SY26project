#include "opencv2/opencv.hpp"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;
using namespace cv;


int main(int narg, char *argv[]) {

  VideoCapture Camera(0);
	Camera.set ( CV_CAP_PROP_FORMAT, CV_64FC3);
	Camera.set ( CV_CAP_PROP_FRAME_WIDTH,  720 );
  Camera.set ( CV_CAP_PROP_FRAME_HEIGHT, 576);
	Camera.set ( CV_CAP_PROP_BRIGHTNESS, 0.60 );
	Camera.set ( CV_CAP_PROP_CONTRAST , 0.65);
	Camera.set ( CV_CAP_PROP_GAIN, 0 );
	Camera.set ( CV_CAP_PROP_EXPOSURE, 0.5 );
  Camera.set ( CV_CAP_PROP_SATURATION, 0.6);

	if (!Camera.isOpened()) {
		cerr<<"Error opening the camera"<<endl;
		return -1;
}
	int kernel_size = 6;
	double sig =1.6, lm = 5, gm = 0.1, ps = 0;

	double theta[4];
	theta[0] = 180;
	theta[1] = 225;
	theta[2] = 270;
	theta[3] = 135;

	// theta[0] = 0 ;
	// theta[1] = CV_PI/4 ;
	// theta[2] = CV_PI/2 ;
	// theta[3] = 3 * CV_PI/4 ;

    Mat frame;

    cout << "on filme" << endl;

    Mat kernel[4];
    for(int i =0; i<4; i++){
		kernel[i] = getGaborKernel(Size(kernel_size,kernel_size), sig, theta[i], lm,gm, ps, CV_64F);
		//cout << kernel[i] << endl;
	}

    for(;;) {
		Camera.grab();
		//Mat sobx_2, soby_2, sobx, soby, result;
		Mat temp[4];
		Mat result;
    Camera.retrieve ( frame);
		imshow("camera init", frame);

    cvtColor(frame,frame,COLOR_BGR2GRAY); //oui ou non ???
    equalizeHist( frame, frame );

    //Sobel(frame, sobx, CV_8U, 1, 0);
    //Sobel(frame, soby, CV_8U, 0, 1);
    //pow(sobx, 2.0, sobx_2);
    //pow(soby_2, 2.0, soby_2);
    //sqrt(sobx + sobx_2, result);
    //result = sobx + soby;

    for(int i =0; i<4; i++){
			filter2D(frame, temp[i], CV_64F, kernel[i]);
			//cout << temp[i] << endl;
			bitwise_not(temp[i], temp[i]);
		}

		Mat temp_2[4];

		 for(int i =0; i<4; i++){
			pow(temp[i], 2.0,  temp_2[i]);
		}

		// imshow("camera0", temp_2[0]);
		// imshow("camera2", temp_2[2]);
		// imshow("camera1", temp_2[1]);
		// imshow("camera3", temp_2[3]);

		sqrt(temp_2[0] + temp_2[1] + temp_2[2] + temp_2[3], result);
    // Mat result2 = (temp[0] + temp[1] +temp[2] + temp[3])/4;


    imshow("frame", frame);

		imshow("camera", result);

        if(waitKey(10) >= 0) break;
    }
    Camera.release();
    cout << "that's all folk" << endl;

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
