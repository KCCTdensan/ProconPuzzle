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

	//画像ファイルからピースを取得(デバッグに便利)
	vector<piece_info> find_pieces(String file_path);

	//くっつけた『『『白黒』』』のピース画像から輪郭の情報を得る
	vector<Point> input_processor::findcontour_from_merged_img(Mat marged_bin_img);

	//枠情報を取得
	piece_info find_frame();
private:
	VideoCapture USBcamera;
	vector<piece_info>find_pieces(Mat);
};