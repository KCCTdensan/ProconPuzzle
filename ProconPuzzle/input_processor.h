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

class input_processor{
public:
	input_processor();

	//USB�J��������s�[�X���擾
	vector<Mat> find_pieces();

	//�摜�t�@�C������s�[�X���擾
	vector<Mat> find_pieces(String file_path);
private:
	VideoCapture USBcamera;
	vector<Mat> find_pieces(Mat);
};