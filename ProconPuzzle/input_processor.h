#pragma once
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

using namespace std;
using namespace cv;

class input_processor{
public:
	input_processor();

	//USBカメラからピースを取得
	vector<Mat> find_pieces();

	//画像ファイルからピースを取得
	vector<Mat> find_pieces(String file_path);
private:
	VideoCapture USBcamera;
	vector<Mat> find_pieces(Mat);
};