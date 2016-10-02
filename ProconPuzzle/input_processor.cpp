#include"input_processor.h"

input_processor::input_processor(){
	USBcamera = VideoCapture(0);
	//UNDONE:解像度は実際のカメラに合わせる
	USBcamera.set(CV_CAP_PROP_FRAME_WIDTH, 960);
	USBcamera.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
}

int canny_arg1 = 200;
int canny_arg2 = 300;
int min_piece_area = 900;
int max_piece_area = 10000;
int corner_detect_threshold_rate = 50;

vector<piece_info> input_processor::find_frame(Mat src_img){
	//UNDONE:
	vector<piece_info> a;
	return  a;
}

vector<piece_info> input_processor::find_pieces(Mat src_img){
	vector<piece_info> pieces;

	Mat gray_srcimg;
	cvtColor(src_img, gray_srcimg, CV_BGR2GRAY);

	medianBlur(gray_srcimg, gray_srcimg, 3);

	Mat bin_img;
	Canny(gray_srcimg, bin_img, canny_arg1, canny_arg2);
	Mat bin_img2 = bin_img.clone();

	vector<vector<Point>> contours;
	findContours(bin_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	Mat result_img(bin_img.size(), CV_8UC3, Scalar(255, 255, 255));
	int piece_counter = 0;
	for(int i = 0; i < contours.size(); ++i){
		Rect piece_rect = boundingRect(contours[i]);
		if(piece_rect.area() > min_piece_area && piece_rect.area() < max_piece_area){
			Mat cut_img(gray_srcimg, piece_rect);
			threshold(cut_img, cut_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
			piece_info pi;
			pi.bin_img = cut_img;
			pi.corners = contours[i];
			pieces.push_back(pi);

			Mat mat = (Mat_<double>(2, 3) << 1.0, 0.0, piece_rect.x, 0.0, 1.0, piece_rect.y);
			warpAffine(cut_img, result_img, mat, result_img.size(), CV_INTER_LINEAR, BORDER_TRANSPARENT);
			rectangle(result_img, piece_rect, Scalar(255, 0, 0));

			approxPolyDP(contours[i], contours[i], std::min(piece_rect.width, piece_rect.height)*corner_detect_threshold_rate / 100, true);
			for(int tpp = 0; tpp < contours[i].size(); ++tpp){
				rectangle(result_img, Rect(contours[i][tpp].x, contours[i][tpp].y, 10, 10), Scalar(128));
			}

			String label_text = to_string(piece_counter);
			putText(result_img, label_text, Point(piece_rect.x, piece_rect.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1, CV_AA);
			piece_counter++;
		}
	}
	//HACK:何度も作る事になる?
	namedWindow("輪郭抽出", CV_WINDOW_AUTOSIZE);
	imshow("輪郭抽出", result_img);
	namedWindow("二値化画像", CV_WINDOW_AUTOSIZE);
	imshow("二値化画像", bin_img2);
	createTrackbar("Canny_arg_1", "二値化画像", &canny_arg1, 1000);
	createTrackbar("Canny_arg_2", "二値化画像", &canny_arg2, 1000);
	createTrackbar("min_piece_area", "輪郭抽出", &min_piece_area, 20000);
	createTrackbar("max_piece_area", "輪郭抽出", &max_piece_area, 20000);
	createTrackbar("corner_detect_threshold_rate", "輪郭抽出", &corner_detect_threshold_rate, 200);

	return pieces;
}

vector<piece_info> input_processor::find_pieces(){
	if(!USBcamera.isOpened()){
		CV_Error(Error::StsError, "USBカメラが使えません!");
	}
	Mat camera_img;
	namedWindow("カメラプレビュー", CV_WINDOW_AUTOSIZE);
	vector<piece_info> pieces;
	while(true){
		USBcamera >> camera_img;
		imshow("カメラプレビュー", camera_img);
		int key_input = waitKey(1);
		vector<piece_info> got_pieces = find_pieces(camera_img);//TODO:破棄する物をわざわざ作ってるのでちょっとアレ
		if(key_input == 't'){//[T]ake a picture
			pieces.insert(pieces.end(), got_pieces.begin(), got_pieces.end());
		} else if(key_input == 'f'){//[F]rame

		} else if(key_input == 'e'){//[E]xit
			break;
		}
	}
	return pieces;
}

vector<piece_info> input_processor::find_pieces(String file_path){
	return find_pieces(imread(file_path));
}