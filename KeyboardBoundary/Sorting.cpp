#include"Header.h"
void cornerSortingY(CvPoint2D32f* corners, int startIndex , int cornerCount){
	CvPoint2D32f tmp;
	for(int i = 0 ; i < cornerCount ; i++){
			for(int j = startIndex ; j < (startIndex + cornerCount -1 ) -i ; j++){
				if(corners[j].y < corners[j+1].y){
					tmp = corners[j];
					corners[j] = corners[j+1];
					corners[j+1] = tmp;
				}	
			}
		}
}
void cornerSortingX(CvPoint2D32f* corners, int startIndex, int cornerCount){
	CvPoint2D32f tmp;
	for(int i = 0  ; i < cornerCount  ; i++){
		for(int j = startIndex ; j < (cornerCount + startIndex -1 ) -i ; j++){
			if(corners[j].x > corners[j+1].x){
				tmp = corners[j];
				corners[j] = corners[j+1];
				corners[j+1] = tmp;
			}	
		}
	}
}
void showPointData(CvPoint2D32f* corners, IplImage* copyImg){
	
	for(int i = 0 ; i < 64 ; i++)
		cvCircle(copyImg, cvPointFrom32f(corners[i]), 2, CV_RGB(255,0,0),3);
	for(int i = 64 ; i < 120 ; i++)
		cvCircle(copyImg, cvPointFrom32f(corners[i]), 2, CV_RGB(255,255,0),3);
	for(int i = 120 ; i < 176 ; i++)
		cvCircle(copyImg, cvPointFrom32f(corners[i]), 2, CV_RGB(0,255,255),3);

	for(int i = 176 ; i < 228 ; i++)
		cvCircle(copyImg, cvPointFrom32f(corners[i]), 2, CV_RGB(0,0,0),3);
	for(int i = 228 ; i < 276 ; i++)
		cvCircle(copyImg, cvPointFrom32f(corners[i]), 2, CV_RGB(170,10,120),3);
	for(int i = 276 ; i < 304 ; i++)
		cvCircle(copyImg, cvPointFrom32f(corners[i]), 2, CV_RGB(255,0,255),3);
	for(int i = 304 ; i < 320 ; i++)
		cvCircle(copyImg, cvPointFrom32f(corners[i]), 2, CV_RGB(90,240,100),3);


}

void initializationKeyboardRegion(CvPoint2D32f* corners){
	cornerSortingY(corners,0,320);

	cornerSortingX(corners, 0,32);
	cornerSortingX(corners, 32,32);

	cornerSortingX(corners, 64,28);
	cornerSortingX(corners, 92,28);

	cornerSortingX(corners, 120,28);
	cornerSortingX(corners, 148,28);

	cornerSortingX(corners, 176, 26);
	cornerSortingX(corners, 202, 26);

	cornerSortingX(corners, 228,24);
	cornerSortingX(corners, 252,24);

	cornerSortingX(corners, 276,44);			//버그 발견
	cornerSortingY(corners, 276, 28);			//버그 발견
	//cornerSortingY(corners, 290, 14);

	cornerSortingX(corners, 276, 14);
	cornerSortingX(corners, 290, 14);

	cornerSortingY(corners, 304, 16);

	cornerSortingX(corners, 304, 2);
	cornerSortingX(corners, 306, 2);

	cornerSortingX(corners, 308, 6);
	cornerSortingX(corners, 314, 6);

	/*setkbRegion(corners, 0, 16);
	setkbRegion(corners, 64, 14);
	setkbRegion(corners, 120, 14);
	setkbRegion(corners, 176, 13);
	setkbRegion(corners, 228, 12);
	setkbRegion(corners, 276, 7);
	setkbRegion(corners, 304, 1);
	setkbRegion(corners, 308, 3);*/



}
void rowSorting(IplImage* img, CvRect kbRegion,CvPoint2D32f* corners){
	IplImage* eig_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U,1);
	IplImage* temp_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* copyImg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U,3);
	IplImage* mask = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);			//Image masking
	IplImage* hsvImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U,3);
	IplImage* test = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* srcImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	//CvPoint2D32f corners[320];			//점들을 정렬하여 저장하기 위한 corners
	cvCopy(img, copyImg);				//카니 엣지는 효율적인 방법이 아니다. 변환 작업에서 이미지 변형 발생
										// Image Masking 작업이 좋은 듯. --> masking 하다가 망함... 일단 넘어가자
	cvCvtColor(copyImg, srcImage, CV_BGR2GRAY);

	int value = 210;
	//cvSplit(copyImg, 0, test, 0 , 0);
	cvInRangeS(copyImg, cvScalar( value , value, value), cvScalar(255, 255, 255), mask);		
	int cornerCount = 320;
	CvRect rect = cvRect(20, 50, widthROI- 40, heightROI - 100);
	cvErode(mask, mask, 0, 1);
	cvDilate(mask, mask, 0, 1);
	cvSetImageROI(srcImage, rect);  
	cvSetImageROI(mask, rect);
	cvSetImageROI(copyImg, rect);
	cvNot(mask, mask);
	cvGoodFeaturesToTrack(srcImage, eig_img, temp_img, corners, &cornerCount, 0.01, minDistance, NULL, 7, 0);
	cvFindCornerSubPix (srcImage, corners, cornerCount,cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
	for(int i = 0; i < 320 ; i++){
		corners[i].x += 20;
		corners[i].y += 50;
	}
	//init
	if(cornerCount == 320)
		initializationKeyboardRegion(corners);
	
	
	cvResetImageROI(copyImg);
	//Show Data
	//	showPointData(corners, copyImg);
	//showRectangleData(copyImg);
	
	//solution 2 for sorting


	cvResetImageROI(mask);
	
	//cvShowImage("frame", copyImg);
	//cvShowImage("mask", mask);
	
	
//	char key = cvWaitKey(0);


	//release 
	cvReleaseImage(&eig_img);
	//cvReleaseImage(&img);
	cvReleaseImage(&copyImg);
	cvReleaseImage(&temp_img);
	cvReleaseImage(&mask);

}