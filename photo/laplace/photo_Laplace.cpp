#include "opencv2/opencv.hpp"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <typeinfo>

using namespace std;
using namespace cv;

void detecte_edges(Mat & img) {
	Mat ep2, ep3;

	int ddepth = CV_8U, ksize = 3;
	double scale = 0.1, delta = 0;

	cvtColor(img,img,COLOR_BGR2GRAY);

	double minVal, maxVal;

	GaussianBlur(img,img, Size(7,7), 1, 1);
	GaussianBlur(img,img, Size(7,7), 1, 1);
	GaussianBlur(img,img, Size(7,7), 1, 1);

	Laplacian(img, ep2, ddepth, ksize, scale, delta, BORDER_DEFAULT );

	minMaxLoc(ep2, &minVal, &maxVal); //find minimum and maximum intensities
	ep2.convertTo(ep3, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
	for(int i = 0; i < 2; i++){
		medianBlur(ep3,ep3, 3);
	}

	adaptiveThreshold(ep3, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 0);
}

int main(int narg, char *argv[]) {

    Mat frame ;
    cout << "on filme" << endl;

    frame = imread(argv[1], CV_LOAD_IMAGE_COLOR );
    if(! frame.data )                              // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }
				imshow("dz", frame);

				detecte_edges(frame);
				resize(frame,frame,Size(28,28));//resize image

    for(;;) {
			imshow("photo", frame);

        if(waitKey(10) >= 0) break;
    }
    cout << "that's all folk" << endl;

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
