#include<opencv2/objdetect.hpp>
#include<opencv2\videoio.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>


#pragma comment(lib, "opencv_world343.lib")


#pragma comment(lib, "opencv_world343d.lib")


#include<iostream>
#include<stdio.h>
#include<Windows.h>

#define WCAM_WIDIH 480
#define WCAM_HEIGHT 480

using namespace cv;
using namespace std;

DWORD oldtime;
DWORD nowtime;


String face_cascade_name, eyes_cascade_name;
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;



bool IsFirstRun = 1;

Mat lena;

void detectAndDisplay_mode1(Mat&frame);

int main() {
	VideoCapture cam(0);
	lena = imread("main.png");
	Mat StartPage(512, 512, CV_8UC3, Scalar(255, 255, 255));
	putText(StartPage, "PRESS AnyKeys", Point(100, 256), 2, 1, Scalar::all(0), 2);
	imshow("game", StartPage); cout << "SHOW START PAGE" << endl;
	waitKey();

	oldtime = GetTickCount();

	if (IsFirstRun) {
		face_cascade_name = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
		eyes_cascade_name = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";


		if (face_cascade.load(face_cascade_name)) { cout << "Loading Face CASEADE SUCCESS" << endl; }
		if (eyes_cascade.load(eyes_cascade_name)) { cout << "Loading Eyes CASEADE SUCCESS" << endl; }
	}


	Mat frame;
	//DWORD time = GetTickCount();
	cam.set(CV_CAP_PROP_FRAME_WIDTH, WCAM_WIDIH);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, WCAM_HEIGHT);
	if (!cam.isOpened()) { cout << "ERROR OPENING CAM" << endl; }
	else cout << "CAM OPEN" << endl;
	try {
		while (cam.read(frame)) {
			if (frame.empty()) {
				cout << "ERROR NOFRAME BREAK" << endl;
				break;
			}

			detectAndDisplay_mode1(frame);

			nowtime = GetTickCount();
			waitKey(1);


		}
	}
	catch (int Game_Code) {
		cout << "¾Ó Àâ¾Ò¶ì " << Game_Code << endl;
		imshow("game", lena);
		char code = waitKey();
		switch (code)
		{

		case 114:
			destroyAllWindows();

			main();
			break;
		case 27:
			return 0;
		default:
			break;
		}
	}
	return 0;


}
void detectAndDisplay_mode1(Mat& frame)
{
	Mat frame_bill;
	Mat frame_gray = frame;
	nowtime = (GetTickCount() - oldtime) / 1000;
	stringstream Totime;
	Totime << nowtime;
	string time = Totime.str();
	cvtColor(frame_gray, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	bilateralFilter(frame_gray, frame_bill, 5, 1500, 1500);

	vector<Rect> faces;
	face_cascade.detectMultiScale(frame_bill, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(150, 150));

	for (int i = 0; i < faces.size(); i++)
		rectangle(frame, faces[i], Scalar(0, 255, 0), 3);

	for (int i = 0; i < faces.size(); i++) {
		vector<Rect> eyes;
		Mat faces_Roi = frame_bill(faces[i]);

		eyes_cascade.detectMultiScale(faces_Roi, eyes, 1.1, 15, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
		cout << eyes.size() << endl;

		if (eyes.size() < 2) {
			putText(lena, time, Point(35, 25), CV_FONT_HERSHEY_COMPLEX, 1.0, Scalar(255, 0, 0), 2);
			throw 0;
		}

		for (int j = 0; j < eyes.size(); j++) {
			Point Center(faces[i].x + eyes[j].x + (eyes[j].width / 2), faces[i].y + eyes[j].y + (eyes[j].height / 2));
			int Radius = cvRound((eyes[j].width + eyes[j].height)*0.2);
			circle(frame, Center, Radius, Scalar(0, 0, 255), 2);
		}

	}
	flip(frame, frame, 1);
	putText(frame, time, Point(10, 25), CV_FONT_HERSHEY_COMPLEX, 1.0, Scalar(255, 0, 0), 3);



	flip(frame_bill, frame_bill, 1);
	imshow("game", frame);
	imshow("test", frame_bill);
}

