#include<opencv/cv.h>
#include<opencv/highgui.h>
#include<stdio.h>
/*
CvRect selection;			//����ڰ� ������ ����.
bool bLButtonDown = false;	//���콺 ���� ��ư�� ����. ������ true;
typedef enum {INIT, CALC_HIST, TRACKING} STATUS;
STATUS trackingMode = INIT;		//
//callback function
void on_mouse(int event, int x, int y, int flag, void* param){
static CvPoint origin;	// ó�� ���콺 ��ư�� ���� ����.
IplImage *image = (IplImage*)param;
if(!image){
printf("Error Occur\n");
return;
}
if(bLButtonDown){
selection.x = MIN(x, origin.x);
selection.y = MIN(y, origin.y);
selection.width = selection.x + CV_IABS(x - origin.x); // ABS�� ���밪
selection.height = selection.y + CV_IABS(y - origin.y);

selection.x = MAX(selection.x,0);
selection.y = MAX(selection.y, 0);
selection.width = MIN(selection.width, image->width);
selection.height = MIN(selection.height, image->height);
selection.width -= selection.x;
selection.height -= selection.y;
}
//�̺�Ʈ �߻��� ���� ó���� ������
switch(event){
case CV_EVENT_LBUTTONDOWN:		// ��ư�� ���ȴٸ�
origin = cvPoint(x, y);		//���� ��ư �ڸ��� �����ϰ�
selection = cvRect(x,y, 0 , 0);		//���� �ڸ��� rect�� �����ϰ�
bLButtonDown =  true;			//��ư�� ���� ���·� �����
break;
case CV_EVENT_LBUTTONUP:				//��ư�� ����
bLButtonDown = false;				// �� ���·� �����
if(selection.width > 0 && selection.height > 0)		//���࿡ ���õ� ������ 0 �̻��̸�
trackingMode = CALC_HIST;				//���õ� ������ ���� histgram�� ���.
break;
}
}
int main(){
CvCapture* capture = cvCaptureFromFile("ball.wmv");		// ���� ������ �о����
if(!capture){
printf("the video file was not found.");			//���� ���������� �����޼��� ����� ����
return -1;
}
int width = (int) cvGetCaptureProperty(capture, 3);		//���� ���� width, height�� ���Ѵ�.
int height = (int) cvGetCaptureProperty(capture, 4);
CvSize size = cvSize(width, height);				//�װ����� size�� ����

IplImage* frame = NULL;								//capture�κ��� ������ frame 
IplImage* hsvImage = cvCreateImage(size, IPL_DEPTH_8U, 3);	//3ä�� ������ 8��Ʈ
IplImage* hImage = cvCreateImage(size, IPL_DEPTH_8U, 1);		//�� ��� ������ 8��Ʈ
IplImage* mask = cvCreateImage(size, IPL_DEPTH_8U, 1);
IplImage* backProject = cvCreateImage(size, IPL_DEPTH_8U, 1);
IplImage* dstImage = cvCreateImage(size, IPL_DEPTH_8U, 3);

int histSize = 8; //������׷��� ������. 8���� ���� �����ϴ�...
float valueRange[] = {0, 180};	//�� ������׷��� x���� 0~180 ���� ���´�.
float* ranges[] = {valueRange};		//����

CvHistogram *pHist = NULL;	// histgram�� ������ pHist�� �����.
pHist = cvCreateHist(1, &histSize, CV_HIST_ARRAY, ranges, 1);	//������ 1����, ũ��� histSize, Ÿ���� array, ������ ranges ������ histogram ����

CvRect track_window; // ����ڰ� �����ϴ� �簢�� ����
CvConnectedComp track_comp;		//track_window�� �̵��Ѱ� ����
CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_EPS |  CV_TERMCRIT_ITER, 10, 5);	//MeanShift�Լ��� ���� ������ ����

cvNamedWindow("dstImage");	//windowâ�ϳ��� �̸� ����
cvSetMouseCallback("dstImage", on_mouse, dstImage);	//dstImage â�� on_mouse callback �Լ��� ����.

float max_val;
CvPoint pt1, pt2;
int k, hValue;
char chKey;
CvBox2D track_Box;
for(int t = 0 ;; t++){
frame = cvQueryFrame(capture);	//capture�� ���� frame �ϳ��� �����´�.

if(!frame)
break;
cvCvtColor(frame, hsvImage, CV_BGR2HLS);		//frame�� hsv�̹����� ����ȯ
cvCopy(frame, dstImage);						//dstImage�� frame �̹����� �����Ѵ�. 
if(bLButtonDown && selection.width > 0 && selection.height > 0){
//��ư�� ���Ȱ�, width�� height�� �þ�� �ִٸ�	--> ������ �����ϰ� �ִٸ�
cvSetImageROI(dstImage, selection);			//
cvXorS(dstImage, cvScalarAll(255), dstImage, 0);
cvResetImageROI(dstImage);
}
if(trackingMode){
// CALC_HIST �� TRACKING ��� �϶�
int vmin = 50, vmax = 256, smin = 50;
cvInRangeS(hsvImage, cvScalar(0, smin, MIN(vmin, vmax)), cvScalar(180, 256, MAX(vmin, vmax)), mask); 
// �����Ϸ��� �÷��� ������ cvScalar(0, smin, MIN(vmin, vmax)) ~ cvScalar(180, 256, MAX(vmin, vmax))���̷� �����Ͽ�
// ���� ���� ȭ�Ҵ� 255, �������� 0���� �Ͽ� ����ŷ�Ѵ�.
// �� ����ڰ� ������ ������ ������ 255, �������� 0���� masking �Ǿ�� �Ѥ�.
cvSplit(hsvImage, hImage,0,0,0); // hsv�� Hä�� �̹����� himage�� ����.
if(trackingMode == CALC_HIST){
//���� ����ڰ� ���콺�� ���� histogram ����ϴ� �۾�.
// ����ũ �̹����� �����.
cvSetImageROI(hImage, selection);	//������ ������ ���� ROI�� ����.
cvSetImageROI(mask, selection);		//masking �� �̹����� ROI�� ����
cvCalcHist(&hImage, pHist, 0 , mask); //pHist���� �����ϰ��� �ϴ� ��ü�� Hü���� ������׷��� ���. 
cvGetMinMaxHistValue(pHist,0,&max_val, 0, 0);								//pHist���� �ִ밪�� ���ϰ�,
cvScale(pHist->bins , pHist->bins, max_val ? 255. / max_val : 0.0);			//0,255�� ������ �����ϸ�

cvResetImageROI(hImage);			//ROI�� �������ϰ�
cvResetImageROI(mask);
track_window = selection;			//track_window�� ������ ������
trackingMode = TRACKING;

}
cvCalcBackProject(&hImage, backProject, pHist);
cvAnd(backProject, mask, backProject);				//backProject�� mask�� ��Ʈ �����Ͽ� �����Ϸ��� ������ ��ġ������ ���� �����ϰ� �������� 0�� �ǰ�

cvCamShift(backProject, track_window, criteria, &track_comp,&track_Box);

track_window = track_comp.rect;						//
pt1 = cvPoint(track_window.x, track_window.y);
pt2 = cvPoint(pt1.x + track_window.width, pt1.y + track_window.height);

//cvRectangle(dstImage, pt1, pt2, CV_RGB(0,0,255), 3);


cvEllipseBox(dstImage, track_Box, CV_RGB(100,100,100), 3, CV_AA, 0);
}
cvShowImage("backProject", backProject);
cvShowImage("hsvImage", hsvImage);
cvShowImage("hImage", hImage);
cvShowImage("mask", mask);
cvShowImage("dstImage", dstImage);
char chkey = cvWaitKey(50);
if(chkey == 27)
break;
}

cvDestroyAllWindows();
cvReleaseCapture(&capture);
cvReleaseHist(&pHist);
cvReleaseImage(&hsvImage);
cvReleaseImage(&hImage);
cvReleaseImage(&backProject);
cvReleaseImage(&dstImage);
cvReleaseImage(&mask);
return 0;
}*/
/*
int main(){
CvCapture* capture = cvCreateCameraCapture(1);
IplImage *frame;
int width = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
int height = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
CvSize size = cvSize(width, height);
CvVideoWriter* videoWriter = cvCreateVideoWriter("Capture.avi", CV_FOURCC('D', 'I', 'V', 'X'), 24.0, size, 1); 
if(!videoWriter){
printf("Video Writer Error Occur\n");
return -1;
}
while(1){
frame = cvQueryFrame(capture);
CvSize size = cvGetSize(frame);
if(!frame)
break;
cvFlip(frame, frame, 1);
cvShowImage("Show", frame);
cvWriteFrame(videoWriter, frame);
char key = cvWaitKey(30);
if(key == 27)
break;
}
cvReleaseCapture(&capture);
return 0;
}*/
/*
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main (void)
{
int i, corner_count = 500;
IplImage *dst_img1, *dst_img2, *src_img_gray;
IplImage *eig_img, *temp_img;
CvPoint2D32f corners[500];

//image file
char imagePath[256] = "C:\\Users\\����\\Desktop\\IMG_3172.JPG";
printf("%s\n", imagePath);

dst_img1 = cvLoadImage (imagePath, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
dst_img2 = cvCloneImage (dst_img1);
src_img_gray = cvLoadImage (imagePath, CV_LOAD_IMAGE_GRAYSCALE);
eig_img = cvCreateImage (cvGetSize (src_img_gray), IPL_DEPTH_32F, 1);
temp_img = cvCreateImage (cvGetSize (src_img_gray), IPL_DEPTH_32F, 1);
//corners = (CvPoint2D32f *) cvAlloc (corner_count * sizeof (CvPoint2D32f));

// (1)Corner detection using cvCornerMinEigenVal
cvGoodFeaturesToTrack (src_img_gray, eig_img, temp_img, corners, &corner_count, 0.1, 15, NULL, 3, 0);
cvFindCornerSubPix (src_img_gray, corners, corner_count,
cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
// (2)Draw the detected corner
for (i = 0; i < corner_count; i++)
cvCircle (dst_img1, cvPointFrom32f (corners[i]), 3, CV_RGB (0, 255, 0), 2);

//Message for debugging
printf("MinEigenVal corner count = %d\n", corner_count);

// (3)Corner detection using cvCornerHarris
corner_count = 500;
cvGoodFeaturesToTrack (src_img_gray, eig_img, temp_img, corners, &corner_count, 0.1, 15, NULL, 3, 1, 0.01);
cvFindCornerSubPix (src_img_gray, corners, corner_count,
cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
// (4)Draw the detected corner
for (i = 0; i < corner_count; i++)
cvCircle (dst_img2, cvPointFrom32f (corners[i]), 3, CV_RGB (0, 0, 255), 2);

//Message for debugging
printf("Harris corner count = %d\n", corner_count);

// (5)Display the result
cvNamedWindow ("EigenVal", CV_WINDOW_AUTOSIZE);
cvShowImage ("EigenVal", dst_img1);
cvNamedWindow ("Harris", CV_WINDOW_AUTOSIZE);
cvShowImage ("Harris", dst_img2);
cvWaitKey (0);

cvDestroyWindow ("EigenVal");
cvDestroyWindow ("Harris");
cvReleaseImage (&dst_img1);
cvReleaseImage (&dst_img2);
cvReleaseImage (&eig_img);
cvReleaseImage (&temp_img);
cvReleaseImage (&src_img_gray);

return 0;
}*/

#include<opencv/cv.h>
#include<opencv/highgui.h>
int main(){
	//CvCapture* capture = cvCaptureFromFile("Capture.avi");
	CvCapture* capture = cvCreateCameraCapture(0);
	IplImage* frame;

	int width = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	int height = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	CvSize size = cvSize(width, height);
	CvPoint2D32f corners[400];
	int corner_count = 400;

	IplImage* src_img_gray = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* eig_img = cvCreateImage (size, IPL_DEPTH_32F, 1);
	IplImage* temp_img = cvCreateImage (size, IPL_DEPTH_32F, 1);
	while(1){
		frame = cvQueryFrame(capture);
		if(!frame)
			break;
		cvCvtColor(frame, src_img_gray, CV_BGR2GRAY);
		
			cvGoodFeaturesToTrack (src_img_gray, eig_img, temp_img, corners, &corner_count, 0.1, 7, NULL, 3, 0);
			cvFindCornerSubPix (src_img_gray, corners, corner_count,cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));

			for (int i = 0; i < corner_count; i++)
				cvCircle (frame, cvPointFrom32f (corners[i]), 3, CV_RGB (0, 255, 0), 2);
		
		cvShowImage ("EigenVal", frame);

		char key = cvWaitKey(30);
		if(key == 27)
			break;
	
	}
	cvReleaseImage(&src_img_gray);
	cvReleaseImage(&temp_img);
	cvReleaseImage(&eig_img);
	cvReleaseCapture(&capture);

	return 0;

}
/*
#include<stdio.h>
#include<opencv\cv.h>
#include<opencv\highgui.h>
int main(){
CvCapture* capture = cvCreateCameraCapture(1);
IplImage* frame;
int width = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
int height = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
CvSize size = cvSize(width, height);
CvVideoWriter* videoWriter = cvCreateVideoWriter("Capture.avi", CV_FOURCC('D', 'I', 'V', 'X'), 24.0, size, 1); 
if(!videoWriter){
printf("Video Writer Error Occur\n");
return -1;
}

while(1){
frame = cvQueryFrame(capture);
if(!frame)
break;
cvFlip(frame, frame, 1);
cvShowImage("Test",frame);
cvWriteFrame(videoWriter, frame);

char key = cvWaitKey(30);
if(key == 27)
break;
}
return 0;
}*/