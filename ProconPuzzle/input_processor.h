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

struct piece_info{
	Mat bin_img;
	vector<Point> corners;
};

class input_processor{
public:
	input_processor();

	//USBカメラからピースを取得
	vector<piece_info> find_pieces();

	//画像ファイルからピースを取得
	vector<piece_info> find_pieces(String file_path);

	piece_info find_frame();
private:
	VideoCapture USBcamera;
	vector<piece_info>find_pieces(Mat);
};