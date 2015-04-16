#include"Header.h"
int camTest(){

	//CvCapture* capture = cvCaptureFromFile("Capture.avi");
	CvCapture* capture = cvCreateCameraCapture(0);
	if(!capture){
		printf("Error Occur\n");
		return -1;
	}
	CvSize size = getCaptureSize(capture);

	IplImage* src_img_gray = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* eig_img = cvCreateImage (size, IPL_DEPTH_32F, 1);
	IplImage* temp_img = cvCreateImage (size, IPL_DEPTH_32F, 1);
	IplImage* frame = NULL;

	CvRect rect = cvRect(80, 100, 500, 300);
	CvPoint pt1, pt2;

	CvPoint2D32f corners[350];
	int corner_count = 350;
	char key;
	int i;

	while(1){
		frame = cvQueryFrame(capture);
		if(!frame)
			break;
		cvCvtColor(frame, src_img_gray, CV_BGR2GRAY);
		cvSetImageROI(frame, rect);
		cvSetImageROI(src_img_gray, rect);


		cvGoodFeaturesToTrack(src_img_gray, eig_img, temp_img, corners, &corner_count, 0.01, 8, NULL, 7, 0);
		cvFindCornerSubPix (src_img_gray, corners, corner_count,cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
		for (i = 0; i < corner_count; i++){
			cvCircle (frame, cvPointFrom32f (corners[i]), 3, CV_RGB (0, 255, 0), 2);

		}
		if(corner_count == 320){
	//		cornerIDentified(corners);
		}
		cvResetImageROI(frame);
		cvResetImageROI(src_img_gray);
		pt1 = cvPoint(rect.x, rect.y);
		pt2 = cvPoint(pt1.x + rect.width, pt1.y + rect.height);
		cvRectangle(frame, pt1, pt2, CV_RGB(0,255,0), 2);

		cvShowImage ("EigenVal", frame);

		key = cvWaitKey(30);
		if(key == 27)
			break;

	}
	cvReleaseImage(&src_img_gray);
	cvReleaseImage(&temp_img);
	cvReleaseImage(&eig_img);
	cvReleaseCapture(&capture);

	return 0;


}