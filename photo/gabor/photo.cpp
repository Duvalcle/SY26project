#include "opencv2/opencv.hpp"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

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
	img = (img - min) / (max - min);
	// cout << "\nmin : " << min << "\t max : " << max << endl;
}

int main(int narg, char *argv[]) {

	int kernel_size = 11;
	double sig = 1.5, lm = 4, gm = 0.3, ps = 0;
	double theta[4];
	// theta[0] = 360;
	// theta[1] = 45;
	// theta[2] = 90+180;
	// theta[3] = 135- 180;

	theta[0] = 0 ;
	theta[1] = CV_PI/4 ;
	theta[2] = CV_PI/2 ;
	theta[3] = 3 * CV_PI/4 ;

    Mat frame;
//
    cout << "on filme" << endl;

    Mat kernel[4], prKer[4];
    for(int i =0; i<4; i++){
			kernel[i] =getGaborKernel(Size(kernel_size,kernel_size), sig, theta[i], lm,gm, ps, CV_32F);
			// cout << kernel[i] << "\n" << endl;
			// if (i == 0 or i == 2){
			// 	kernel[i] -= 0.5;
			// }
			resize(kernel[i], prKer[i], cv::Size(), 10, 10);
			regulate(prKer[i]);
			// double min, max;
			// minMaxLoc(prKer[i], &min, &max);
			// cout << "min : " << min << "\t max : " << max << endl;
			// prKer[i] = (prKer[i] - min) / (max - min);
	  }

		//Mat sobx_2, soby_2, sobx, soby, result;
		Mat temp[4];
		Mat result;
    frame = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED );
    if(! frame.data )                              // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }

    cvtColor(frame,frame,COLOR_BGR2GRAY); //oui ou non ???
		// troncate(frame);
		// regulate(frame);
		double min, max;

		minMaxLoc(frame, &min, &max);

		cout << "\nmin : " << min << "\t max : " << max << endl;

    // equalizeHist( frame, frame );
    //Sobel(frame, sobx, CV_8U, 1, 0);
    //Sobel(frame, soby, CV_8U, 0, 1);
    //pow(sobx, 2.0, sobx_2);
    //pow(soby_2, 2.0, soby_2);
    //sqrt(sobx + sobx_2, result);
    //result = sobx + soby;

    for(int i =0; i<4; i++){
			filter2D(frame, temp[i], CV_32F, kernel[i]);
			//cout << temp[i] << endl;
			// double min, max;
			// threshold(temp[i], temp[i], 1, 1, THRESH_TRUNC);
			// threshold(temp[i], temp[i], 0, 0, THRESH_TOZERO);
			troncate(temp[i]);
			// minMaxLoc(temp[i], &min, &max);
			// cout << "\nmin : " << min << "\t max : " << max << endl;
			//
			// temp[i] = (temp[i] - min) / (max - min);

			// minMaxLoc(temp[i], &min, &max);
			// cout << "min : " << min << "\t max : " << max << endl;

			// im - min / (max - min)
			// bitwise_not(temp[i], temp[i]);
		}


		Mat temp_2[4];

		 for(int i =0; i<4; i++){
			pow(temp[i], 2.0,  temp_2[i]);
			// sqrt(temp[i],temp_2[i]);
		}

		// imshow("camera0", temp_2[0]);
		// imshow("camera2", temp_2[2]);
		// imshow("camera1", temp_2[1]);
		// imshow("camera3", temp_2[3]);
		bitwise_and(temp_2[0], temp_2[1], result);
		bitwise_and(result, temp_2[2], result);
		bitwise_and(result, temp_2[3], result);

		// double min, max;
		// pow(temp_2[0] + temp_2[1] + temp_2[2] + temp_2[3], 2.0, result);
		// sqrt(temp_2[0] + temp_2[1] + temp_2[2] + temp_2[3], result);
		// minMaxLoc(result, &min, &max);
		// result = (result - min) / (max - min);

		// cout << "\nmin : " << min << "\t max : " << max << endl
		// result = (temp_2[0] + temp_2[1] + temp_2[2] + temp_2[3])/4;

		// regulate(result);
		// troncate(result);
		// double min, max;
		// minMaxLoc(result, &min, &max);
		// result = (result - min) / (max - min);
		// cout << "\nmin : " << min << "\t max : " << max << endl;


    // imshow("camera_moy", result2);
		// bitwise_not(result2, result2);

    for(;;) {
			imshow("kernel0", prKer[0]);
			imshow("kernel1", prKer[1]);
			imshow("kernel2", prKer[2]);
			imshow("kernel3", prKer[3]);

			// imshow("camera0", temp[0]);
			// imshow("camera1", temp[1]);
			// imshow("camera3", temp[3]);
			// imshow("camera2", temp[2]);
			//
			// imshow("camera0_2", temp_2[0]);
			// imshow("camera2_2", temp_2[2]);
			// imshow("camera1_2", temp_2[1]);
			// imshow("camera3_2", temp_2[3]);
    	imshow("photo_base", frame);
			imshow("photo", result);

        if(waitKey(10) >= 0) break;
    }
    cout << "that's all folk" << endl;

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
