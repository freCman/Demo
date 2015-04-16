#include"Header.h"

int imageTest(){
	IplImage *img = cvLoadImage("Test.png");
	IplImage* src_img_gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* hsvImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	IplImage* eig_img = cvCreateImage (cvGetSize(img), IPL_DEPTH_32F, 1);
	IplImage* temp_img = cvCreateImage (cvGetSize(img), IPL_DEPTH_32F, 1);
	IplImage* test = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* copyImg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	IplImage *dstImg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	CvRect rect;
	CvPoint pt1, pt2;
	CvPoint2D32f corners[350]; 
	CvPoint2D32f pt[4];
	CvPoint2D32f srcQuad[4], dstQuad[4];
	CvMat* warp_matix = cvCreateMat(3,3, CV_32FC1);
	IplImage *src, *dst;
	int corner_count = 2;
	cvZero(test);
	cvCopy(img, copyImg);
	cvCvtColor(img, hsvImage, CV_BGR2HSV);
	cvSplit(hsvImage, 0,src_img_gray,0,0);
	cvGoodFeaturesToTrack(src_img_gray, eig_img, temp_img, corners, &corner_count, 0.05, 50, NULL, 7, 0);
	cvFindCornerSubPix (src_img_gray, corners, corner_count,cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));

	
	pt1 = cvPoint((int)(corners[0].x + 10) , (int)(corners[0].y +30));
	pt2 = cvPoint((int)(corners[1].x -10) , (int)(corners[1].y));
	
	rect = cvRect(pt2.x, pt2.y -230, pt1.x - pt2.x, 270);
	
	cvSetImageROI(img, rect);
	cvSetImageROI(hsvImage, rect);
	cvSetImageROI(src_img_gray, rect);
	cvCvtColor(img, hsvImage, CV_BGR2HSV);
	cvSplit(hsvImage, 0, src_img_gray, 0,0);
	
	corner_count = 4;
	cvErode(src_img_gray, src_img_gray, 0, 3);
	cvGoodFeaturesToTrack(src_img_gray, eig_img, temp_img, corners, &corner_count, 0.05, 50, NULL, 7, 0);
	cvFindCornerSubPix (src_img_gray, corners, corner_count,cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
	

	for (int i = 0; i < corner_count; i++){
			cvCircle (img, cvPointFrom32f (corners[i]), 3, CV_RGB (255, 0, 0), 2);
			pt[i] = corners[i];
	}

		
	srcQuad[0] = pt[2];						//11시
	srcQuad[1] = pt[3];		//1시
	srcQuad[2] = pt[0];						//7시
	srcQuad[3] = pt[1];			//5시
				//위 좌표를

	dstQuad[0].x = 0;
	dstQuad[0].y = 0;
	dstQuad[1].x = pt1.x - pt2.x;
	dstQuad[1].y = 0;
	dstQuad[2].x = 0;
	dstQuad[2].y = cvGetSize(img).height;
	dstQuad[3].x = pt1.x - pt2.x;
	dstQuad[3].y = 270;		//이렇게 바꾸려고
	cvGetPerspectiveTransform(srcQuad, dstQuad, warp_matix);
	cvWarpPerspective(img, img, warp_matix);

	cvShowImage("Test", img);
	cvShowImage("kk", copyImg);
	
	char key = cvWaitKey(0);

	cvReleaseImage(&img);

	return 0;
}