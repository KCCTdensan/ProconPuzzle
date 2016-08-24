#include<stdio.h>
#include<iostream>

//TODO:opencv2は?
#include <opencv/cv.hpp>
//TODO:必要なの?
#include <opencv/cxcore.hpp>
//TODO:必要なの?
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

int main(){
	Mat src_img = imread("pzl.png");

	Mat gray_srcimg;
	cvtColor(src_img, gray_srcimg, CV_BGR2GRAY);

	medianBlur(gray_srcimg, gray_srcimg, 3);

	Mat bin_img;
	Canny(gray_srcimg, bin_img, 200, 300);

	vector<vector<Point>> contours;
	findContours(bin_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat result_img(bin_img.size(), CV_8UC3, Scalar(255, 255, 255));
	int piece_counter = 0;
	for(int i = 0; i < contours.size(); ++i){
		Rect piece_rect = boundingRect(contours[i]);
		if(piece_rect.area() > 900 && piece_rect.area() < 10000){
			drawContours(result_img, contours, i, Scalar(0, 0, 0), -1);
			rectangle(result_img, piece_rect, Scalar(255, 0, 0));

			String label_text = to_string(piece_counter);
			putText(result_img, label_text, Point(piece_rect.x, piece_rect.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1, CV_AA);
			piece_counter++;
		}
	}

	namedWindow("hntykr", CV_WINDOW_AUTOSIZE);
	imshow("hntykr", result_img);

	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(50);
	imwrite("res.png", result_img);// , compression_params);

	waitKey();
	destroyAllWindows();
}