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

	//�摜�t�@�C������s�[�X���擾(�f�o�b�O�ɕ֗�)
	vector<piece_info> find_pieces(String file_path);

	//���������w�w�w�����x�x�x�̃s�[�X�摜����֊s�̏��𓾂�
	vector<Point> input_processor::findcontour_from_merged_img(Mat marged_bin_img);

	//�g�����擾
	piece_info find_frame();
private:
	VideoCapture USBcamera;
	vector<piece_info>find_pieces(Mat);
};