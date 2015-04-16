#include"Header.h"
void getVideo(){
	CvCapture* capture = cvCreateCameraCapture(1);
	IplImage* frame;
	CvVideoWriter* videoWriter = cvCreateVideoWriter("Capture10.avi", CV_FOURCC('D','I','V','X'), 24.0, getCaptureSize(capture), 1);
	if(!videoWriter)
		return;
	while(1){
		frame = cvQueryFrame(capture);
		if(!frame)
			break;
		cvFlip(frame, frame, 1);

		cvShowImage("frame", frame);
		cvWriteFrame(videoWriter, frame);
		char key = cvWaitKey(30);
		if(key == 27)
			break;
	}
	cvReleaseCapture(&capture);

}