#include"input_processor.h"

input_processor::input_processor(){
	USBcamera = VideoCapture(0);
	//UNDONE:解像度は実際のカメラに合わせる
	USBcamera.set(CV_CAP_PROP_FRAME_WIDTH, 960);
	USBcamera.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
}

vector<Mat> input_processor::find_pieces(Mat src_img){
	vector<Mat> pieces;

	Mat gray_srcimg;
	cvtColor(src_img, gray_srcimg, CV_BGR2GRAY);

	medianBlur(gray_srcimg, gray_srcimg, 3);

	Mat bin_img;
	Canny(gray_srcimg, bin_img, 200, 300);//HACK:マジックナンバー
	Mat bin_img2 = bin_img.clone();

	vector<vector<Point>> contours;
	findContours(bin_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat result_img(bin_img.size(), CV_8UC3, Scalar(255, 255, 255));
	int piece_counter = 0;
	for(int i = 0; i < contours.size(); ++i){
		Rect piece_rect = boundingRect(contours[i]);
		if(piece_rect.area() > 900 && piece_rect.area() < 10000){//HACK:マジックナンバー
			Mat cut_img(piece_rect.size(), CV_8U);
			drawContours(cut_img, contours, i, Scalar(0), CV_FILLED, 8, noArray(), 0, Point(-piece_rect.x, -piece_rect.y));
			pieces.push_back(cut_img);

			drawContours(result_img, contours, i, Scalar(0, 0, 0), CV_FILLED);
			rectangle(result_img, piece_rect, Scalar(255, 0, 0));

			String label_text = to_string(piece_counter);
			putText(result_img, label_text, Point(piece_rect.x, piece_rect.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1, CV_AA);
			piece_counter++;
		}
	}
	namedWindow("hntykr2", CV_WINDOW_AUTOSIZE);
	imshow("hntykr2", result_img);
	namedWindow("hntykr3", CV_WINDOW_AUTOSIZE);
	imshow("hntykr3", bin_img2);

	return pieces;
}

vector<Mat> input_processor::find_pieces(){
	if(!USBcamera.isOpened()){
		CV_Error(Error::StsError, "The USBcamera is not available.");
	}
	Mat img;
	USBcamera >> img;
	return find_pieces(img);
}

vector<Mat> input_processor::find_pieces(String file_path){
	return find_pieces(imread(file_path));
}