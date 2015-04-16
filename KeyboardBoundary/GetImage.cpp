#include"Header.h"
void getImage(){
	CvCapture * capture = cvCreateCameraCapture(0);
	IplImage *frame;

	while(1){
		frame = cvQueryFrame(capture);

		if(!frame)
			break;
		cvFlip(frame, frame, 1);
		cvShowImage("Frame", frame);

		char key = cvWaitKey(30);
		if(key == 27)
			break;
	}

	cvReleaseCapture(&capture);
}