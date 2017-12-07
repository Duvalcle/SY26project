#include "opencv2/opencv.hpp"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;
using namespace cv;

void troncate(Mat &img) {
	threshold(img, img, 1, 1, THRESH_TRUNC);
	threshold(img, img, 0, 0, THRESH_TOZERO);
}

void regulate(Mat &img) {
	double min, max;
	minMaxLoc(img, &min, &max);
	img = (img - min) / (max - min);
	cout << "\nmin : " << min << "\t max : " << max << endl;
}

int main(int narg, char *argv[]) {

  VideoCapture Camera(0);
	Camera.set ( CV_CAP_PROP_FORMAT, CV_8UC3);
	Camera.set ( CV_CAP_PROP_FRAME_WIDTH,  720 );
  Camera.set ( CV_CAP_PROP_FRAME_HEIGHT, 576);
	Camera.set ( CV_CAP_PROP_BRIGHTNESS, 0.55 );
	Camera.set ( CV_CAP_PROP_CONTRAST , 0.75);
	Camera.set ( CV_CAP_PROP_GAIN, 0 );
	Camera.set ( CV_CAP_PROP_EXPOSURE, 0.2 );
  Camera.set ( CV_CAP_PROP_SATURATION, 0.6);

	if (!Camera.isOpened()) {
		cerr<<"Error opening the camera"<<endl;
		return -1;
}
    Mat frame, ep2 ,ep3 ;
		double minVal, maxVal;

		int ddepth = CV_8U, ksize = 3;
		double scale = 0.1, delta = 0;

    cout << "on filme" << endl;

    for(;;) {
		Camera.grab();
    Camera.retrieve ( frame);
		imshow("camera init", frame);

    cvtColor(frame,frame,COLOR_BGR2GRAY); //oui ou non ???



		// medianBlur(frame,frame, 9);
		GaussianBlur(frame,frame, Size(7,7), 1, 1);
		GaussianBlur(frame,frame, Size(7,7), 1, 1);
		GaussianBlur(frame,frame, Size(7,7), 1, 1);


			Laplacian(frame, ep2, ddepth, ksize, scale, delta, BORDER_DEFAULT );

			minMaxLoc(ep2, &minVal, &maxVal); //find minimum and maximum intensities
			// regulate(ep3);
			ep2.convertTo(ep3, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
			for(int i = 0; i < 2; i++){
				medianBlur(ep3,ep3, 3);
				// GaussianBlur(ep3,ep3, Size(3,3), 1, 1);

				// boxFilter(ep3,ep3, -1, Size(3,3), Point(-1,-1), true, BORDER_DEFAULT);
			}

			// medianBlur(ep3,frame, 3);
		// adaptiveThreshold(frame, frame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 0);
		adaptiveThreshold(ep3, ep3, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 0);






		// for(int i = 0; i < 10; i++){
		// 	medianBlur(frame,frame, 3);
		// 	// boxFilter(frame,frame, -1, Size(3,3), Point(-1,-1), true, BORDER_DEFAULT);
		// }
		// // medianBlur(frame,frame, 11);
		// // fastNlMeansDenoising(frame,frame, 21, 7);
		// Laplacian(frame, result, ddepth, ksize, scale, delta, BORDER_DEFAULT );
		//
		// minMaxLoc(result, &minVal, &maxVal); //find minimum and maximum intensities
		//
		// result.convertTo(draw_result, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
		//
		// // minMaxLoc(draw_result, &minVal, &maxVal);
		//
		// // cout << "\nmin : " << minVal << "\t max : " << maxVal << endl;
		//
		// threshold(draw_result, temp, 150, 255, THRESH_BINARY);
		// adaptiveThreshold(draw_result, temp_2, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 0);

    imshow("frame", frame);

		// imshow("camera_befor", ep2);
		imshow("camera", ep3);

        if(waitKey(10) >= 0) break;
    }
    Camera.release();
    cout << "that's all folk" << endl;

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
