#include"input_processor.h"

const int camera_img_width=960;
const int camera_img_height=720;

input_processor::input_processor(){
	USBcamera = VideoCapture(0);
	//UNDONE:�𑜓x�͎��ۂ̃J�����ɍ��킹��
	USBcamera.set(CV_CAP_PROP_FRAME_WIDTH, camera_img_width);
	USBcamera.set(CV_CAP_PROP_FRAME_HEIGHT, camera_img_height);
}

int canny_arg1 = 200;
int canny_arg2 = 300;
int min_piece_area = 900;
int max_piece_area = 10000;
int corner_detect_threshold_rate = 50;

vector<Point> input_processor::findcontour_from_merged_img(Mat marged_bin_img){
		Mat bin_img;
		Canny(marged_bin_img, bin_img, 0, 0);//HACK:��肭�s���Ȃ�����
		vector<vector<Point>> contours;
		findContours(bin_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		
		//approxPolyDP(contours[0], contours[0], std::min(marged_bin_img.cols, marged_bin_img.rows) * 1 / 100, true);
		approxPolyDP(contours[0], contours[0], arcLength(contours[0], true) / contours[0].size()*2.5, true);//HACK:

		return contours[0];
}

piece_info input_processor::find_frame() {
	piece_info pieces;
	while (true) {
		Mat src_img;
		USBcamera >> src_img;


		Mat gray_srcimg;
		cvtColor(src_img, gray_srcimg, CV_BGR2GRAY);

		medianBlur(gray_srcimg, gray_srcimg, 3);

		Mat bin_img;
		Canny(gray_srcimg, bin_img, canny_arg1, canny_arg2);
		Mat bin_img2 = bin_img.clone();

		//dilate(bin_img, bin_img, Mat(), Point(-1, -1), 1);
		//erode(bin_img, bin_img, Mat(), Point(-1, -1), 1);

		vector<vector<Point>> contours;
		findContours(bin_img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
		Mat result_img(bin_img.size(), CV_8UC3, Scalar(255, 255, 255));
		if(contours.size() > 0){
			int min_distance = INT_MAX;
			int min_distance_index = 0;
			for(int i = 0; i < contours.size(); ++i){
				Rect piece_rect = boundingRect(contours[i]);
				if(piece_rect.area() > min_piece_area){
					//�s�[�X�O�ڋ�`���摜���S��荶�と�g�����ۂ�
					if(piece_rect.x < camera_img_width / 2 && piece_rect.y < camera_img_height / 2){
						//�s�[�X�O�ڋ�`���摜���S�𒴂���
						if(piece_rect.x + piece_rect.width > camera_img_width / 2 && piece_rect.y + piece_rect.height > camera_img_height / 2){
							//�s�[�X�O�ڋ�`���ォ��摜���S�܂ł̋�������ԒZ������Ԙg�����ۂ�!
							int distance = pow(piece_rect.x - camera_img_width / 2, 2) + pow(piece_rect.y - camera_img_height / 2, 2);
							if(min_distance > distance){
								min_distance = distance;
								min_distance_index = i;
							}
						}
					}
				}
			}
			Mat cut_img(gray_srcimg, boundingRect(contours[min_distance_index]));
			threshold(cut_img, cut_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
			piece_info pi;
			pi.bin_img = cut_img;
			pi.corners = contours[min_distance_index];
			pieces = pi;

			Mat mat = (Mat_<double>(2, 3) << 1.0, 0.0, boundingRect(contours[min_distance_index]).x, 0.0, 1.0, boundingRect(contours[min_distance_index]).y);
			warpAffine(cut_img, result_img, mat, result_img.size(), CV_INTER_LINEAR, BORDER_TRANSPARENT);
			rectangle(result_img, boundingRect(contours[min_distance_index]), Scalar(255, 0, 0));
			approxPolyDP(contours[min_distance_index], contours[min_distance_index], std::min(boundingRect(contours[min_distance_index]).width, boundingRect(contours[min_distance_index]).height)*corner_detect_threshold_rate / 100, true);
			for(int tpp = 0; tpp < contours[min_distance_index].size(); ++tpp){
				rectangle(result_img, Rect(contours[min_distance_index][tpp].x, contours[min_distance_index][tpp].y, 10, 10), Scalar(128));
			}
		}
		//HACK:���x����鎖�ɂȂ�?
		namedWindow("�t���[���֊s���o", CV_WINDOW_AUTOSIZE);
		imshow("�t���[���֊s���o", result_img);
		namedWindow("�t���[����l���摜", CV_WINDOW_AUTOSIZE);
		imshow("�t���[����l���摜", bin_img2);
		createTrackbar("Canny_arg_1", "�t���[����l���摜", &canny_arg1, 1000);
		createTrackbar("Canny_arg_2", "�t���[����l���摜", &canny_arg2, 1000);
		createTrackbar("min_piece_area", "�t���[���֊s���o", &min_piece_area, 250000);
		createTrackbar("corner_detect_threshold_rate", "�t���[���֊s���o", &corner_detect_threshold_rate, 200);
		int key_input = waitKey(1);
		if (key_input == 't') {
			break;
		}
	}
	destroyWindow("�t���[���֊s���o");
	destroyWindow("�t���[����l���摜");
	return pieces;
}

int picture_n;
Mat save_image;//HACK:�ő��d���Ȃ�

vector<piece_info> input_processor::find_pieces(Mat src_img) {
	vector<piece_info> pieces;

	Mat gray_srcimg;
	cvtColor(src_img, gray_srcimg, CV_BGR2GRAY);

	//medianBlur(gray_srcimg, gray_srcimg, 3);

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

	save_image = result_img;


	//HACK:���x����鎖�ɂȂ�?
	namedWindow("�֊s���o", CV_WINDOW_AUTOSIZE);
	imshow("�֊s���o", result_img);
	namedWindow("��l���摜", CV_WINDOW_AUTOSIZE);
	imshow("��l���摜", bin_img2);
	createTrackbar("Canny_arg_1", "��l���摜", &canny_arg1, 1000);
	createTrackbar("Canny_arg_2", "��l���摜", &canny_arg2, 1000);
	createTrackbar("min_piece_area", "�֊s���o", &min_piece_area, 150000);
	createTrackbar("max_piece_area", "�֊s���o", &max_piece_area, 150000);
	createTrackbar("corner_detect_threshold_rate", "�֊s���o", &corner_detect_threshold_rate, 200);

	return pieces;
}

vector<piece_info> input_processor::find_pieces(){
	if(!USBcamera.isOpened()){
		CV_Error(Error::StsError, "USB�J�������g���܂���!");
	}
	Mat camera_img;
	namedWindow("�J�����v���r���[", CV_WINDOW_AUTOSIZE);
	vector<piece_info> pieces;
	picture_n = 0;
	while(true){
		USBcamera >> camera_img;
		imshow("�J�����v���r���[", camera_img);
		int key_input = waitKey(1);
		vector<piece_info> got_pieces = find_pieces(camera_img);//TODO:�j�����镨���킴�킴����Ă�̂ł�����ƃA��
		if(key_input == 't'){//[T]ake a picture
			pieces.insert(pieces.end(), got_pieces.begin(), got_pieces.end());
			imwrite(to_string(picture_n) + ".png", save_image);
			picture_n++;
		} else if(key_input == 'f'){//[F]rame

		} else if(key_input == 'e'){//[E]xit
			break;
		}
	}
	destroyWindow("�֊s���o");
	destroyWindow("��l���摜");
	return pieces;
}

vector<piece_info> input_processor::find_pieces(String file_path){
	return find_pieces(imread(file_path));
}