#include"Header.h"
void drawRect(IplImage * img){
	CvRect rect = cvRect(90, 170, widthROI, heightROI);
	IplImage *copy = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);


	cvCopy(img, copy);
	cvSetImageROI(copy, rect);

	//cvRectangle(copy, cvPointFrom32f (firstRow[0][1]), cvPointFrom32f(firstRow[1][0]), CV_RGB(255,0,0), CV_FILLED);
	//cvRectangle(copy, cvPointFrom32f (firstRow[0][0]), cvPointFrom32f(firstRow[1][1]), CV_RGB(255,255,255), CV_FILLED); 
	
	//cvRectangle(copy, cvPointFrom32f (firstRow[1][0]), cvPoint(100,100), CV_RGB(255,0,0),2);
	
	CvPoint pt1 = cvPoint(rect.x, rect.y);
	CvPoint pt2 = cvPoint(rect.x + rect.width, rect.y + rect.height);
	
	
	//cvResetImageROI(copy);
	cvShowImage("sss", copy);
	char key = cvWaitKey(0);
	






}