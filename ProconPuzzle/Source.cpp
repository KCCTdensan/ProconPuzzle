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

	//medianBlur(gray_srcimg, gray_srcimg, 3);

	Mat bin_img;
	//TODO:第[34]引数の意味具体的には何か調べる
	Canny(gray_srcimg, bin_img, 200, 300);
	//threshold(gray_srcimg, bin_img, 110, 255, CV_THRESH_BINARY);

	vector<vector<cv::Point>> contours;
	findContours(bin_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//TODO:初期化必須か調べる(自動的に形式判断してくれる?)
	Mat result_img(bin_img.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	int piece_counter = 0;
	for(int i = 0; i < contours.size(); ++i){
		drawContours(result_img, contours, i, Scalar(255, 0, 0));
		//double area = contourArea(contours[i]);
		Rect piece_rect = boundingRect(contours[i]);
		if(piece_rect.area() > 400 && piece_rect.area() < 10000){
			rectangle(result_img, boundingRect(contours[i]), Scalar(0, 0, 255));

			//HACK:この変換羅列一纏めにしちゃって見やすくしよう
			Mat cut_img(src_img, piece_rect);
			Mat gray_cut_img;
			cvtColor(cut_img, gray_cut_img, CV_BGR2GRAY);
			Mat bin_cut_img;
			//TODO:AdaptiveThreshold様子見
			threshold(gray_cut_img, bin_cut_img, 110, 255, CV_THRESH_BINARY);
			Mat mat = (cv::Mat_<double>(2, 3) << 1.0, 0.0, piece_rect.x, 0.0, 1.0, piece_rect.y);
			//TODO:result_img.size()何故必要か調べる
			warpAffine(bin_cut_img, result_img, mat, result_img.size(), CV_INTER_LINEAR, BORDER_TRANSPARENT);

			//HACK:FIXMEでもある．warpAffineの前にputTextすると文字が消える謎
			ostringstream os;
			os << piece_counter;
			string label_text = os.str();
			putText(result_img, label_text, Point(piece_rect.x, piece_rect.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 1, CV_AA);
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