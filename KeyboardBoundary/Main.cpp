#include"Header.h"

int main(){
	generate();
	//getVideo();
	//imageTest();
	/*
	IplImage* originImage = cvLoadImage("Test.png");

	CvSize size = cvGetSize(originImage);
	IplImage* hsvImage = cvCreateImage(size, IPL_DEPTH_8U, 3);
	IplImage* mask = cvCreateImage(size, IPL_DEPTH_8U, 1);
	CvRect kbRegion = cvRect(0, size.height/3 + 20, size.width, 2*(size.height/3) -20 );
	IplImage* sImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* dstImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	
	cvCvtColor(originImage, hsvImage, CV_BGR2HSV);
	
	cvSplit(hsvImage, 0, sImage, 0, 0);
	cvInRangeS(hsvImage, cvScalar(0,40,40), cvScalar(100,255,255), mask);

	cvDilate(sImage, sImage, 0, 5);
	cvErode(sImage, sImage, 0, 5);
	cvDilate(mask,mask, 0,5);
	cvErode(mask,mask,0,5);
	cvAnd(mask, sImage, dstImage);

	IplImage* eig_img = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* temp_img = cvCreateImage(size, IPL_DEPTH_8U, 1);
	CvPoint2D32f corners[4];
	int cornerCount = 4;

	//cvSetImageROI(dstImage, kbRegion);
	//cvSetImageROI(originImage, kbRegion);

	cvGoodFeaturesToTrack(dstImage, eig_img, temp_img, corners, &cornerCount, 0.1, 200, NULL, 7, 0);
	cvFindCornerSubPix (dstImage, corners, cornerCount,cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
	for(int i = 0; i < cornerCount ; i++)
		cvCircle(originImage, cvPointFrom32f(corners[i]), 3, CV_RGB(255,0,0), 3);

	//cvResetImageROI(dstImage);
	//cvResetImageROI(originImage);
	cvRectangleR(originImage, kbRegion, CV_RGB(0,255,0), 2);
	
	
	
	cvShowImage("3", dstImage);
	cvShowImage("Test", originImage);

	char key = cvWaitKey(0);

	cvReleaseImage(&mask);
	cvReleaseImage(&hsvImage);
	cvReleaseImage(&originImage);
	*/


	return 0;
}