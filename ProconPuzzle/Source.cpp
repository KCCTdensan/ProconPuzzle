#include<stdio.h>
#include<iostream>
//TODO:opencv2は?
#include <opencv/cv.hpp>
//TODO:必要なの?
#include <opencv/cxcore.hpp>
//TODO:必要なの?
#include <opencv/highgui.h>
//TODO:必要なの?
#include <opencv2/opencv.hpp>

#include"input_processor.h"

using namespace std;
using namespace cv;

int main(){
	input_processor inp;
	while(true){
		vector<Mat> pieces = inp.find_pieces();

		Mat resultimg(100, 1000, CV_8UC3);
		int x = 0;
		for(int i = 0; i < pieces.size(); ++i){
			//Mat roi1(resultimg, Rect(0, 0, pieces[i].cols, pieces[i].rows));
			//pieces[i].copyTo(roi1);
			Mat mat = (Mat_<double>(2, 3) << 1.0, 0.0, x, 0.0, 1.0, 0);
			warpAffine(pieces[i], resultimg, mat, resultimg.size(), CV_INTER_LINEAR, BORDER_TRANSPARENT);
			x += pieces[i].cols;
		}
		namedWindow("hntykr", CV_WINDOW_AUTOSIZE);
		imshow("hntykr", resultimg);
		waitKey(1);
	}

	//vector<int> compression_params;
	//compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	//compression_params.push_back(50);
	//imwrite("res.png", pieces[0]);// , compression_params);

	destroyAllWindows();
}