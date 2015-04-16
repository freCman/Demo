#include"Header.h"
void sortingPoint(IplImage* img){
//	rowSorting(img);
}
int imageSort(){
	IplImage *img = cvLoadImage("image.png");
	IplImage* hsvImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	CvRect rect = cvRect(90, 170, widthROI, heightROI);
	CvPoint pt1, pt2;
	CvPoint2D32f corners[320]; 
	int cornerCount = 320;
	float maxY;
	int max;
	
	cvSetImageROI(img, rect);
	sortingPoint(img);
	cvResetImageROI(img);
	

	pt1 = cvPoint(rect.x, rect.y);
	pt2 = cvPoint(rect.x + rect.width, rect.y + rect.height);

	cvReleaseImage(&img);
	cvReleaseImage(&img);
	return 0;
}