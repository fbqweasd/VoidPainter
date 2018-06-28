#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdio.h> 
#include <sqlite3.h>
#include <deque>
#include <math.h>
using namespace cv;
using namespace std; 

vector<Scalar> Colors;
Mat draw;
Mat HSV;
Mat frame;
Mat Bindraw;
Mat UI;
Mat TempBin = Mat(480, 640, CV_8UC1, Scalar(0));
int prex, prey;
int Tswitch = 0;
bool F_flag = false;
int fx, fy;
bool S_flag = false;
Rect folow;

string filename;

bool palette_sw ;

int Linethickness = 2;
int ColorIndex = 0;

Scalar White = Scalar(255, 255, 255);
Scalar Black = Scalar(0, 0, 0);
Scalar Blue = Scalar(255, 0, 0);
Scalar Skyblue = Scalar(255, 150, 120);
Scalar Red = Scalar(0, 0, 255);
Scalar Pink = Scalar(120, 100, 255);
Scalar Green = Scalar(0, 255, 0);
Scalar Lightgreen = Scalar(120, 255, 120);
Scalar Eraser = Scalar(0, 0, 0);


deque<Mat> im;
Mat image2;
int idx = 0;

void LinesDraw(int x,int y){
	//cout << "x = " << x << endl;
	//cout << "y = " << y << endl;
	//cout << endl;
	
	if (true){//(Tswitch == 1) {
		line(draw, Point(prex, prey), Point(x, y), Colors.at(ColorIndex), Linethickness);
		line(Bindraw, Point(prex, prey), Point(x, y), 0, Linethickness);
		prex = x;
		prey = y;
	}
}
void Erase(int x, int y) {
	//cout << "x = " << x << endl;
	//cout << "y = " << y << endl;
	//cout << endl;

	if (true) {//(Tswitch == 1) {
		line(draw, Point(prex, prey), Point(x, y), 0, Linethickness*2);
		line(Bindraw, Point(prex, prey), Point(x, y), Scalar(255,255,255), Linethickness*2);
		prex = x;
		prey = y;
	}
}
/*
void onChange(int value,void* ) {
	Linethickness = value;
}
*/
static void getThreshold(int, void*){}

void greengreen() { //green 이진화
	TempBin = Scalar(0);
	int j;
	for (int i = 0; i < 470; i++) {
		for (j = 0; j < 630; j++) {
			if (HSV.at<Vec3b>(i, j)[0] >= 70 && HSV.at<Vec3b>(i, j)[0] <= 80 && HSV.at<Vec3b>(i, j)[1] >= 80 && HSV.at<Vec3b>(i, j)[2] >= 40) {
				TempBin.at<uchar>(i, j) = 255;
			}
		}
	}
}

void getGreen(){// 전체 그린 찾ㄱ기
	int th = 0;
	//TempBin = Scalar(0);
	int j = 0;
	for (int i = 0; i < 480; i++) {
		for (j = 0; j < 640; j++) {
			if (HSV.at<Vec3b>(i, j)[0] >= 70 && HSV.at<Vec3b>(i, j)[0] <= 80 && HSV.at<Vec3b>(i, j)[1] >= 80 && HSV.at<Vec3b>(i, j)[2] >= 40){
				//TempBin.at<uchar>(i, j) = 255;
				th++;
			}
			else {
				th = 0;
			}
			if (th > 8) {
				cout << "found green in" << j <<","<< i << endl;
				circle(frame,Point(j,i),5,Scalar(0,255,0),2);
				F_flag = true;
				fx = j;
				fy = i;

				prex = fx;
				prey = fy;
				return ;
			}
		}
	}
	F_flag = false;
	//frame.at<Vec3b>;
}

void roigetGreen() { // 영역 그린 찾기
	int ran = 25;
	//rectangle(frame, Rect(fx - ran, fy - ran, ran*2, ran*2), Scalar(0, 255, 0), 1);
	if (fy - ran < 0)
		fy = ran;
	if (fx - ran < 0)
		fx = ran;
	if (fy + ran > frame.rows) {
		fy = frame.rows - ran ;
	}	
	if (fx + ran > frame.cols) {
		fx = frame.cols - ran;
	}

	int th = 0;
	//TempBin = Scalar(0);
	int j = fx - ran;
	for (int i = fy - ran; i < fy+ran; i++) {
		for (j = 0; j < fx+ran; j++) {
			if (HSV.at<Vec3b>(i, j)[0] >= 68 && HSV.at<Vec3b>(i, j)[0] <= 82 && HSV.at<Vec3b>(i, j)[1] >= 80 && HSV.at<Vec3b>(i, j)[2] >= 40) {
				//TempBin.at<uchar>(i, j) = 255;
				th++;
			}
			else {
				th = 0;
			}
			if (th > 5) {
				fx = j;
				fy = i;
				circle(frame, Point(j, i), Linethickness, Scalar(0, 255, 0), 2);
				return;
			}
		}
	}
	F_flag = false;
	cout << "lost green "<< endl;
	cout << " " << endl;
	//frame.at<Vec3b>;
}
void roigetRed() { // 영역 RED 찾기
	int ran = 25;
	//rectangle(frame, Rect(fx - ran, fy - ran, ran * 2, ran * 2), Scalar(0, 0, 255), 1);
	if (fy - ran < 0)
		fy = ran;
	if (fx - ran < 0)
		fx = ran;
	if (fy + ran > frame.rows) {
		fy = frame.rows - ran;
	}
	if (fx + ran > frame.cols) {
		fx = frame.cols - ran;
	}
	int th = 0;
	//TempBin = Scalar(0);
	int j = fx - ran;
	for (int i = fy - ran; i < fy + ran; i++) {
		for (j = fx - ran; j < fx + ran; j++) {
			if (HSV.at<Vec3b>(i, j)[0] >= 175 && HSV.at<Vec3b>(i, j)[0] <= 182 && HSV.at<Vec3b>(i, j)[1] >= 110 && HSV.at<Vec3b>(i, j)[2] >= 40) {
				//TempBin.at<uchar>(i, j) = 255;
				th++;
			}
			else {
				th = 0;
			}
			if (th > 5) {
				cout << "found red" << endl;
				circle(frame, Point(j, i), 5, Scalar(0, 0, 255), 2);
				S_flag = true;
				Tswitch = 0;
				return;
			}
		}
	}
	if(S_flag)
		getGreen();
	S_flag = false;
}

void Drawing() {
	LinesDraw(fx, fy);
}

void Ico_init(Mat bin_) {
	for (int i = 0; i < bin_.rows; i++) {
		for (int j = 0; j < bin_.cols; j++) {
			if (bin_.at<Vec3b>(j, i) == Vec3b(0, 0, 0)) {
				bin_.at<Vec3b>(j, i) = Vec3b(255, 255, 255);
			}else {
				bin_.at<Vec3b>(j, i) = Vec3b(0,0,0);
			}
		}
	}

}

static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;



	for (i = 0; i<argc; i++) {

		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if (i == 1) {
			char tmp[256];
			sprintf(tmp, "C:\\VoidPainter\\%s.jpg", argv[i]);
			cout << tmp << endl;
			Mat image = imread(tmp, IMREAD_COLOR);
			CV_Assert(image.data);
			im.push_back(image);

			//imshow(argv[i], image);
		}
	}

	printf("\n");
	return 0;
}



int insertDB(const char *name) {
	sqlite3 *db;

	char *zErrMsg = 0;
	int rc;


	rc = sqlite3_open("data.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		cout << "파일 미 오픈";
		return -1;
	}


	rc = sqlite3_exec(db, "CREATE TABLE 'images' ( 'No' INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 'filePath' TEXT NOT NULL UNIQUE); ", callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		cout << "예상되는 에러" << endl;
	}

	char tmp[256];
	sprintf(tmp, "INSERT INTO images(filePath) VALUES('%s'); ", name);
	rc = sqlite3_exec(db, tmp, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		cout << "중복이미지" << endl;
		return 0;
	}

	sqlite3_close(db);

	return 1;


}


int getDB() {
	sqlite3 *db;

	char *zErrMsg = 0;
	int rc;


	rc = sqlite3_open("data.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		cout << "파일 미 오픈";
		return -1;
	}
	const char* data = "Callback function called";
	rc = sqlite3_exec(db, "SELECT * from images", callback, (void*)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		cout << "error" << endl;
		return 0;
	}
	sqlite3_close(db);
	return 1;
}

void onImage() {
	for (int i = 0; i < im[idx].rows; i++) {
		for (int j = 0; j < im[idx].cols; j++) {
			for (int k = 0; k < 3; k++) {
				image2.at<Vec3b>(i + 230, j + 320)[k] = im[idx].at<Vec3b>(i, j)[k];
			}
		}
	}
	return;
}

void onMouse(int event, int x, int y, int flags, void *parameter) {


	if (event == EVENT_LBUTTONDOWN) {
		cout << x << ", " << y;
		if (x >= 100 && x <= 315 && y >= 360 && y <= 600) {
			idx = max(0, idx - 1);
			onImage();
			cout << "왼" << endl;
		}
		if (x >= 970 && x <= 1195 && y >= 360 && y <= 600) {
			int a = im.size();
			idx = min(a, idx + 1);
			onImage();
			cout << "오" << endl;
		}
		cout << "마우스 왼쪽 누르기" << endl;




		imshow("이미지 미리보기", image2);
	}
}

int getImage() {



	for (int i = 0; i < im[0].rows; i++) {
		for (int j = 0; j < im[0].cols; j++) {
			for (int k = 0; k < 3; k++) {
				image2.at<Vec3b>(i + 230, j + 320)[k] = im[0].at<Vec3b>(i, j)[k];
			}
		}
	}
	imshow("이미지 미리보기", image2);
	setMouseCallback("이미지 미리보기", onMouse, 0);


	return 0;

}

void palette_on() {
	if (palette_sw == false) {

		frame = (frame & Bindraw) + draw;

		cout << "Palette" << endl;
		palette_sw = true;
		cout << "저장할 사진 이름 입력"<< endl ;
		cin >> filename;
		int t = insertDB(filename.c_str());
		if (t == -1) {
			cout << "error";
		}
		else if (t == 0) {
			cout << "중복된 이미지";
		}
		else if (t == 1) {
			cout << "정상작동";
			imwrite("C:\\VoidPainter\\"+filename+".jpg",frame);
		};


	}
}

int main(){
	int value = 2;
	Colors.push_back(White);
	Colors.push_back(Black);
	Colors.push_back(Blue);
	Colors.push_back(Skyblue);
	Colors.push_back(Red);
	Colors.push_back(Pink);
	Colors.push_back(Green);
	Colors.push_back(Lightgreen);
	Colors.push_back(Eraser);
	
	VideoCapture video(0);
	video >> frame;

	namedWindow("input", WINDOW_AUTOSIZE);

	resize(frame, draw, Size(frame.cols, frame.rows));
	draw = Scalar(0, 0, 0);
	resize(draw, Bindraw, Size(frame.cols, frame.rows));
	Bindraw = Scalar(255,255,255);

	
	createTrackbar("thickness", "input", &value, 20, getThreshold);
	setTrackbarPos("thickness", "input", 2);	
	createTrackbar("color", "input", &value, Colors.size()-1, getThreshold);
	setTrackbarPos("color", "input", 0);
	
	cvtColor(frame, HSV, COLOR_BGR2HSV);

	Rect palette = Rect(10, 10, 60, 60);
	Mat palette_ico = imread("C:\\opcv\\brush.jpg");
	Mat bin_palette;
	Mat palette_roi;
	resize(palette_ico, bin_palette, Size(palette_ico.cols, palette_ico.rows));
	Ico_init(bin_palette);

	/*
	Rect blue = Rect(80, 80, 60, 60);
	Mat blue_ico = imread("C:\\opcv\\brush.jpg");
	Mat bin_blue;
	Mat blue_roi;
	resize(blue_ico, bin_blue, Size(60,60));
	Ico_init(bin_blue);
	*/

	insertDB("test3");
	image2 = imread("C:\\opcv\\select.jpg", IMREAD_COLOR);
	CV_Assert(image2.data);
	getDB();
	getImage();


	bool button = true;

	while (true) {
		video >> frame;
		flip(frame, frame, 1);
		if (waitKey(50) == 27) {
			break;
		}
		cvtColor(frame, HSV, COLOR_BGR2HSV); 
		palette_roi = frame(palette);
		palette_roi = (palette_roi & bin_palette) + palette_ico;

		if (F_flag == true) {
			roigetGreen();
		}else {
			getGreen();
		}

		if (F_flag == true) {
			roigetRed();
		}
		//cout << F_flag << endl;
		//greengreen();
		
		Linethickness = getTrackbarPos("thickness", "input");
		ColorIndex = getTrackbarPos("color", "input");
		

		button = false;
		if (F_flag == true) {
			if (palette.contains(Point(fx, fy))) {
				palette_on();
				button = true;
			}
			else {
				palette_sw = false;
			}
		}

		if (button == false) {
			if (F_flag == true && S_flag == false) {
				if (ColorIndex != 8) {
					Drawing();
				}
				else {
					Erase(fx, fy);
				}
			}
		}
		
		frame = (frame & Bindraw)+draw;
		/*
		imshow("in", palette_ico);
		imshow("inpt", bin_palette);
		*/

		imshow("input", frame);
		//imshow("Bindraw", TempBin);
		//imshow("draw", draw);
	}

	return 0;
}