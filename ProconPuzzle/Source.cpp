#include<stdio.h>
#include<iostream>

#include"input_processor.h"

using namespace std;
using namespace cv;

int main(){
	input_processor inp;
	while(true){
		piece_info frame = inp.find_frame();
		vector<piece_info> pieces = inp.find_pieces();

		Mat resultimg(100, 1500, CV_8UC3);
		int x = 0;
		for(int i = 0; i < pieces.size(); ++i){
			//Mat roi1(resultimg, Rect(0, 0, pieces[i].cols, pieces[i].rows));
			//pieces[i].copyTo(roi1);
			Mat mat = (Mat_<double>(2, 3) << 1.0, 0.0, x, 0.0, 1.0, 0);
			warpAffine(pieces[i].bin_img, resultimg, mat, resultimg.size(), CV_INTER_LINEAR, BORDER_TRANSPARENT);
			x += pieces[i].bin_img.cols;
		}
		Mat mat = (Mat_<double>(2, 3) << 1.0, 0.0, x, 0.0, 1.0, 0);
		warpAffine(frame.bin_img, resultimg, mat, resultimg.size(), CV_INTER_LINEAR, BORDER_TRANSPARENT);

		namedWindow("検出されたピース", CV_WINDOW_AUTOSIZE);
		imshow("検出されたピース", resultimg);
		waitKey(1);
	}

	//vector<int> compression_params;
	//compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	//compression_params.push_back(50);
	//imwrite("res.png", pieces[0]);// , compression_params);

	destroyAllWindows();
}