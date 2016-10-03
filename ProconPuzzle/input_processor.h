#pragma once
#include<stdio.h>
#include<iostream>
//TODO:opencv2��?
#include <opencv/cv.hpp>
//TODO:�K�v�Ȃ�?
#include <opencv/cxcore.hpp>
//TODO:�K�v�Ȃ�?
#include <opencv/highgui.h>
//TODO:�K�v�Ȃ�?
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

	//USB�J��������s�[�X���擾
	vector<piece_info> find_pieces();

	//�摜�t�@�C������s�[�X���擾
	vector<piece_info> find_pieces(String file_path);

	piece_info find_frame();
private:
	VideoCapture USBcamera;
	vector<piece_info>find_pieces(Mat);
};