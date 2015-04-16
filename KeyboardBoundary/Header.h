#ifndef HEADER_H
#define HEADER_H
#include<opencv/cv.h>
#include<opencv/highgui.h>
#include<stdio.h>


const int widthROI = 700, heightROI = 600;
const int minDistance = 10;
CvSize getCaptureSize(CvCapture* capture);
void cornerIDentified(CvPoint2D32f* corners);
int camTest();
int imageTest();
void getVideo();
void generate();
void drawRect(IplImage * img);
void getImage();
int imageSort();
void cornerSortingY(CvPoint2D32f* corners, int startIndex , int cornerCount);
void cornerSortingX(CvPoint2D32f* corners, int startIndex, int cornerCount);
void rowSorting(IplImage* img, CvRect Region, CvPoint2D32f *corners);
void showPointData(CvPoint2D32f* corners, IplImage* copyImg);
void showRectangleData(IplImage* copyImg);
void setkbRegion(CvPoint2D32f* corners, int startIndex, int keyCount);
void initializationKeyboardRegion(CvPoint2D32f* corners);

#endif