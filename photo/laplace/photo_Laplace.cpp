#include "opencv2/opencv.hpp"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <typeinfo>

using namespace std;
using namespace cv;



void troncate(Mat &img) {
	// double min, max;
	threshold(img, img, 1, 1, THRESH_TRUNC);
	threshold(img, img, 0, 0, THRESH_TOZERO);
	// minMaxLoc(img, &min, &max);
	// cout << "\nmin : " << min << "\t max : " << max << endl;
}

void regulate(Mat &img) {
	double min, max;
	minMaxLoc(img, &min, &max);
	img = 255*(img - min) / (max - min);
	// cout << "\nmin : " << min << "\t max : " << max << endl;
}

int main(int narg, char *argv[]) {

    Mat frame, ep2, ep3 ;

		int ddepth = CV_8U, ksize = 3;
		double scale = 0.1, delta = 0;

    cout << "on filme" << endl;

    frame = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED );
    if(! frame.data )                              // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }

    cvtColor(frame,frame,COLOR_BGR2GRAY);
		// for(int i = 0; i < 10; i++){
		// 	medianBlur(frame,frame, 3);
		// 	// boxFilter(frame,frame, -1, Size(3,3), Point(-1,-1), true, BORDER_DEFAULT);
		// }
		// equalizeHist(frame,frame);

		double minVal, maxVal;

		// medianBlur(frame,frame, 9);
		GaussianBlur(frame,frame, 9, 1, 1);


			Laplacian(frame, ep2, ddepth, ksize, scale, delta, BORDER_DEFAULT );

			minMaxLoc(ep2, &minVal, &maxVal); //find minimum and maximum intensities
			// regulate(ep3);
			ep2.convertTo(ep3, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
			for(int i = 0; i < 1; i++){
				// medianBlur(ep3,ep3, 3);
				GaussianBlur(ep3,ep3, 3, 1, 1);

				// boxFilter(ep3,ep3, -1, Size(3,3), Point(-1,-1), true, BORDER_DEFAULT);
			}
			// medianBlur(ep3,frame, 3);
		// adaptiveThreshold(frame, frame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, 0);
		adaptiveThreshold(ep3, ep3, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 0);
		// Laplacian(frame, result, ddepth, ksize, scale, delta, BORDER_DEFAULT );
		// // cout << typeid(result.val[0]).name() << endl;
		// double min, max;
		//
		// minMaxLoc(result, &min, &max);
		//
		// cout << "\nmin : " << min << "\t max : " << max << endl;
		//
		//
		// double minVal, maxVal;
		// minMaxLoc(result, &minVal, &maxVal); //find minimum and maximum intensities
		// Mat draw_result;
		// result.convertTo(draw_result, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
		//
		//
		// minMaxLoc(draw_result, &min, &max);
		//
		// cout << "\nmin : " << min << "\t max : " << max << endl;
		//
		// // for(int i = 0; i < 10; i++){
		// // 	medianBlur(frame,frame, 3);
		// // 	// boxFilter(frame,frame, -1, Size(3,3), Point(-1,-1), true, BORDER_DEFAULT);
		// // }
		//
		// threshold(draw_result, temp, 200, 255, THRESH_BINARY);
		// // adaptiveThreshold(draw_result, temp, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 0);

    for(;;) {
			imshow("photo", frame);
    	// imshow("photo_before", ep2);
			imshow("photo_draw", ep3);
			// imshow("photo", ep1);

        if(waitKey(10) >= 0) break;
    }
    cout << "that's all folk" << endl;

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
