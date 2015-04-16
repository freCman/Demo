#include"Header.h"
CvRect mouseSelect;
typedef enum {SET_ROI,ROI_CONFIRM, SET_CORNER, ADJUST_CORNER, CATCH_CORNER, CORNER_MOVE, CORNER_CONFIRM, CALC_SPOT, CATCH_SKIN_COLOR, CALC_HIST,HAND_TRACKING, OTHER} Status;
typedef enum {Direction_11, Direction_1, Direction_7, Direction_5};
Status mode = SET_ROI;
bool bLButtonDown = false;
bool bRButtonDown = false;
CvPoint2D32f corners[4];
CvPoint2D32f cornersSorting[4];
int index;
CvPoint2D32f point;
CvRect kbRegion;
CvRect kbRegionInfo[6][32];
CvRect kbDirectionRegionInfo[4];
const double MHI_DURATION = 1.0;
const double MAX_TIME_DELTA = 0.5, MIN_TIME_DELTA = 0.05;
void showRectangleData(IplImage* copyImg){
	for(int i =0 ; i < 16 ; i++)
		cvRectangleR(copyImg, kbRegionInfo[0][i], CV_RGB(255,0,0), 2);
	for(int i =0 ; i < 14 ; i++)
		cvRectangleR(copyImg, kbRegionInfo[1][i], CV_RGB(255,255,0), 2);
	for(int i =0 ; i < 14 ; i++){
	/*	if(i == 10)
			cvRectangleR(copyImg, kbRegionInfo[2][i], CV_RGB(0,255,255), CV_FILLED);
		else*/
			cvRectangleR(copyImg, kbRegionInfo[2][i], CV_RGB(0, 255, 255), 2);
	}	
	for(int i =0 ; i < 13 ; i++){
			cvRectangleR(copyImg, kbRegionInfo[3][i], CV_RGB(0,0,0), 2);
	}
	for(int i =0 ; i < 12 ; i++)
		cvRectangleR(copyImg, kbRegionInfo[4][i], CV_RGB(170,10,120), 2);
	for(int i =0 ; i < 7 ; i++)
		cvRectangleR(copyImg, kbRegionInfo[5][i], CV_RGB(255,0,255), 2);
	for(int i =0 ; i < 4 ; i++)
		cvRectangleR(copyImg, kbDirectionRegionInfo[i], CV_RGB(90,240,100), 2);

}
void setkbRegion(CvPoint2D32f* corners, int startIndex, int keyCount){
	int cornerCount = keyCount*2;
	int keyWidth, keyHeight;
	int keyX, keyY;
	float width1, width2;
	int index = 0;
	static int line = 0;
	float height1, height2;
	static int k = 0;
	for(int i =startIndex  ; i < cornerCount + startIndex ; i +=2){
		width1 = corners[i+1].x - corners[i].x;
		width2 = corners[i+cornerCount + 1].x - corners[i + cornerCount].x;
		width1 > width2 ? keyWidth = width1 : keyWidth =  width2;

		height1 = corners[i].y - corners[i + cornerCount].y;
		height2 = corners[i+1].y - corners[i + cornerCount].y;
		height1 > height2 ? keyHeight = height1 : keyHeight = height2;

		keyX = (corners[i + cornerCount].x + corners[i].x)/2;
		keyY = (corners[i + cornerCount].y + corners[i + cornerCount + 1].y)/2;
		if(line < 6)
			kbRegionInfo[line][index++] = cvRect(keyX, keyY, keyWidth, keyHeight);
		else
			kbDirectionRegionInfo[k++] = cvRect(keyX, keyY, keyWidth, keyHeight);
	}
	line++;
}
void setRegion(int event, int x, int y, int flags, void* param){
	static CvPoint origin;
	IplImage* image = (IplImage*) param;
	if(!image)
		return;
	if(mode == SET_ROI && bLButtonDown){
		mouseSelect.x = MIN(x, origin.x);
		mouseSelect.y = MIN(y, origin.y);
		mouseSelect.width = mouseSelect.x + CV_IABS(x - origin.x);
		mouseSelect.height = mouseSelect.y + CV_IABS(y- origin.y);

		mouseSelect.x = MAX(mouseSelect.x, 0);
		mouseSelect.y = MAX(mouseSelect.y, 0);
		mouseSelect.width = MIN(mouseSelect.width, image->width);
		mouseSelect.height = MIN(mouseSelect.height, image->height);
		mouseSelect.width -= mouseSelect.x;
		mouseSelect.height -= mouseSelect.y;
	}
	if(mode == ADJUST_CORNER && bRButtonDown){
		mouseSelect.x = MIN(x, origin.x);
		mouseSelect.y = MIN(y, origin.y);
		mouseSelect.width = mouseSelect.x + CV_IABS(x - origin.x);
		mouseSelect.height = mouseSelect.y + CV_IABS(y- origin.y);

		mouseSelect.x = MAX(mouseSelect.x, 0);
		mouseSelect.y = MAX(mouseSelect.y, 0);
		mouseSelect.width = MIN(mouseSelect.width, image->width);
		mouseSelect.height = MIN(mouseSelect.height, image->height);
		mouseSelect.width -= mouseSelect.x;
		mouseSelect.height -= mouseSelect.y;

	}
	switch(event){
	case CV_EVENT_LBUTTONDOWN:
		if(mode == SET_ROI){
			origin = cvPoint(x, y);
			mouseSelect = cvRect(x, y, 0, 0);
			bLButtonDown = true;
		}
		if(mode == ROI_CONFIRM){
			printf("ROI�� �����Ǿ����ϴ�.\n");
			mode = SET_CORNER;
		}
		if(mode == CORNER_CONFIRM){
			printf("���� �����Ǿ����ϴ�.\n");
			mode = CALC_SPOT;
		}
		if(mode == CATCH_CORNER){
			bLButtonDown = true;
			if(mouseSelect.x < x && mouseSelect.x + mouseSelect.width > x){
				if(mouseSelect.y < y && mouseSelect.y + mouseSelect.height > y){
					printf("��ġ�� �����մϴ�\n");
					mode = CORNER_MOVE;
				}
			}
		}
		if(mode == CATCH_SKIN_COLOR){
			printf("����� �Ǻ� ���� ����Ͽ� �����մϴ�.\n");
			mode = CALC_HIST;
		}
		break;
	case CV_EVENT_MOUSEMOVE:
		if(mode == CORNER_MOVE && bLButtonDown){
			point.x = (float)(x - kbRegion.x);
			point.y = (float)(y - kbRegion.y);
			corners[index] = point;
		}
		break;
	case CV_EVENT_LBUTTONUP:
		if(mode == SET_ROI){
			bLButtonDown = false;
			if(mouseSelect.width > 0 && mouseSelect.height > 0){
				mode = ROI_CONFIRM;
				printf("ROI�� Ȯ���մϴ�. ������ ����, �缳���� ������ ���콺 ��ư�� Ŭ�����ּ���\n");
			}
		}
		else if(mode == CORNER_MOVE){
			bLButtonDown = false;
			printf("������ ��ġ���� ����Ŭ��, �����Ұ� �� �ִٸ� ���������� ������ ����ּ��� \n");
			mode = CORNER_CONFIRM;
		}
		break;
	case CV_EVENT_RBUTTONDOWN: 
		if(mode == ROI_CONFIRM){
			printf("ROI�� �缳�� �մϴ�.\n");
			mouseSelect.width = 0;
			mouseSelect.height = 0;
			mouseSelect.x = 0;
			mouseSelect.y = 0;
			mode = SET_ROI;
		}
		else if(mode == CORNER_CONFIRM){
			printf("��ġ�� ������ �մϴ�.\n");
			mode = ADJUST_CORNER;
			origin = cvPoint(x, y);
			mouseSelect = cvRect(x, y, 0, 0);
			bRButtonDown = true;
		}
		else if(mode == ADJUST_CORNER){
			origin = cvPoint(x, y);
			mouseSelect = cvRect(x, y, 0, 0);
			bRButtonDown = true;
		}
		break;
	case CV_EVENT_RBUTTONUP:
		if(mode == ADJUST_CORNER){
			bRButtonDown = false;
			printf("%d, %d\n", mouseSelect.x, mouseSelect.y);
			for(int i = 0; i < 4 ; i++){
				if(corners[i].x + kbRegion.x > (mouseSelect.x ) && corners[i].x + kbRegion.x < (mouseSelect.x + mouseSelect.width))
					if(corners[i].y +  kbRegion.y > mouseSelect.y && corners[i].y +  kbRegion.y< (mouseSelect.y + mouseSelect.height)){
						index = i;
						point = corners[i];
						mode = CATCH_CORNER;
						printf("%d���� ���õǾ����ϴ�. ���� ��ư���� �巡�� �Ͽ� ���ϴ� ��ġ�� �ű⼼��\n", index);
					}
			}
		}
	}
}
void cornerSorting(){
	CvPoint2D32f temp;
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 3 - i ; j++){
			if(corners[j].y > corners[j+1].y){
				temp = corners[j];
				corners[j] = corners[j+1];
				corners[j+1] = temp;
			}
		}
	}
	for(int i =0 ; i < 2 ; i++){
		for(int j = 0 ; j < 1 -i ; i++){
			if(corners[j].x > corners[j+1].x){
				temp = corners[j];
				corners[j] = corners[j+1];
				corners[j+1] = temp;
			}
		}
	}
	for(int i = 0 ; i < 2 ; i++){
		for(int j = 0 ; j < 1 -i ; i++){
			if(corners[j+2].x < corners[j+3].x){
				temp = corners[j+2];
				corners[j+2] = corners[j+3];
				corners[j+3] = temp;
			}
		}
	}
}
void DrawMotionOrientation(CvSeq* seq, IplImage* silh, IplImage* mhi,IplImage* orient, IplImage* mask, IplImage* dstImage, double timeStamp, CvRect rect)
{
	int i, x, y;
	int count;
	CvRect comp_rect;
	CvScalar color;
	CvPoint center;
	double r, angle;
	CvSize size = cvGetSize(dstImage);
	int d;
	for( i = 0; i < seq->total; i++ )						//�����̴� ��ü���� ���� ������ �ݺ���
	{

		comp_rect = ((CvConnectedComp*)cvGetSeqElem( seq, i ))->rect;	//sep�� i���� index�� �����ͼ� 
		// reject very small components
		comp_rect.x = comp_rect.x + rect.x;
		comp_rect.y = comp_rect.y + rect.y;
		if( comp_rect.width*comp_rect.height < 50 && comp_rect.width*comp_rect.height < 150)			//�� �����°� �ǰ� ������ 
			continue;			//�Ѿ��
		color = CV_RGB(255,0,0);			//�������� ����
		r = 30;

		// select component ROI
		cvSetImageROI( silh, comp_rect );		//ROI �Ҵ�.
		cvSetImageROI( mhi, comp_rect );
		cvSetImageROI( orient, comp_rect );
		cvSetImageROI( mask, comp_rect );
		// calculate orientation
		angle = cvCalcGlobalOrientation( orient, mask, mhi,  timeStamp, MHI_DURATION);		//������ ������ ����Ͽ� ��ȯ
		// adjust for images with top-left origin
		angle = 360.0 - angle;//angle;					//���� ����� ������ ���� ���� ���� ����.
		count = cvNorm(silh, NULL, CV_L1, NULL );	// silh�� V_L1 ���� ���

		cvResetImageROI( mhi );
		cvResetImageROI( orient );
		cvResetImageROI( mask );
		cvResetImageROI( silh );

		// check for the case of little motion
		if( count < comp_rect.width*comp_rect.height * 1 )		//���� ������ ó�� ����.
			continue;

		// draw a clock with arrow indicating the direction
		center = cvPoint( ( comp_rect.x + comp_rect.width/2),			//ROI������ ���
			( comp_rect.y + comp_rect.height/2) );

		cvCircle( dstImage, center, cvRound(r*1.2), color, 3, CV_AA, 0 );	//�� �׸�.
		x = cvRound( center.x + r*cos(angle*CV_PI/180));			//value�� �ݿø� �Ͽ� ������ ��ȯ.
		y = cvRound( center.y - r*sin(angle*CV_PI/180));
		cvLine( dstImage, center, cvPoint(x, y), color, 3, CV_AA, 0 );	//center���� cvPint(x, y) ���� ���� �׸���.
		if(angle>240 && angle < 300)
			printf("key event occur\n");
	}
}
void generate(){ 
	CvCapture* capture = cvCaptureFromFile("Capture3.avi");
	//CvCapture* capture = cvCreateCameraCapture(0);
	IplImage* frame;
	CvSize size = getCaptureSize(capture);
	IplImage* dstImage = cvCreateImage(size, IPL_DEPTH_8U,3);
	cvNamedWindow("FRAME",1);
	cvSetMouseCallback("FRAME", setRegion, dstImage);
	CvPoint2D32f srcQuad[4], dstQuad[4], keyCorners[320];
	CvMat* warp_matrix = cvCreateMat(3,3, CV_32FC1);
	CvMat* warp_matrix_invert = cvCreateMat(3,3, CV_32FC1);
	CvMat* result = cvCreateMat(3, 1, CV_32FC1);
	CvMat* dst = cvCreateMat(3, 1,CV_32FC1);
	CvPoint center = cvPoint(2*(size.width/3), 2*(size.height/3)-60);
	
	int cornerCount = 4;
	IplImage* YCrCbImage = cvCreateImage(size, IPL_DEPTH_8U, 3);
	IplImage* handMasking = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* CrImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* backProject = cvCreateImage(size, IPL_DEPTH_8U, 1);

	float max;
	int histSize = 8;
	float valueRange[] = {100, 180};
	float* ranges[] = {valueRange};
	CvHistogram *pHist = cvCreateHist(1, &histSize, CV_HIST_ARRAY, ranges, 1); 
	IplImage *trans = cvCreateImage(cvSize(widthROI, heightROI), IPL_DEPTH_8U, 3);
	CvConnectedComp handTracking;
	CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_EPS|CV_TERMCRIT_ITER, 10, 5);
	int hValue;

	IplImage* silh = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* mhi = cvCreateImage(size, IPL_DEPTH_32F, 1);
	IplImage* buffer[4];
	IplImage* motion = cvCreateImage(size, IPL_DEPTH_8U, 3);
	IplImage* moveMask = cvCreateImage(size, IPL_DEPTH_8U,1);
	IplImage* segmask = cvCreateImage(size, IPL_DEPTH_32F, 1);
	IplImage* orient = cvCreateImage(size, IPL_DEPTH_32F, 1);
	double threshold = 50, timeStamp;
	int curr,prev, last = 0;

	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq* seq = NULL;
	if(!capture)
		printf("Camear Error Occur\n");
	cvZero(motion);
	while(1){
		frame = cvQueryFrame(capture);
		if(!frame)
			break;
		cvCopy(frame, dstImage);
		if(mode == SET_ROI && bLButtonDown && mouseSelect.width > 0 && mouseSelect.height > 0){
			cvSetImageROI(dstImage,mouseSelect);
			cvXorS(dstImage, cvScalarAll(255), dstImage, 0);
			cvResetImageROI(dstImage);
		}
		if(mode){
			if(mode == ROI_CONFIRM){
				kbRegion = mouseSelect;
				cvRectangleR(dstImage, kbRegion, CV_RGB(0, 255, 0), 2);
			}
			else if(mode == SET_CORNER){
				IplImage* hsvImage = cvCreateImage(size, IPL_DEPTH_8U, 3);
				IplImage* sImage = cvCreateImage(size, IPL_DEPTH_8U, 1);
				IplImage* mask = cvCreateImage(size, IPL_DEPTH_8U, 1);
				IplImage* objImage = cvCreateImage(size, IPL_DEPTH_8U, 1);

				cvCvtColor(dstImage, hsvImage, CV_BGR2HSV);
				cvSplit(hsvImage, 0, sImage, 0, 0);
				cvInRangeS(hsvImage, cvScalar(0, 40, 40), cvScalar(100, 255, 255), mask);

				cvDilate(sImage, sImage, 0, 5);
				cvErode(sImage, sImage, 0, 5);

				cvDilate(mask, mask, 0, 5);
				cvErode(mask, mask, 0, 5);
				cvAnd(mask, sImage, objImage);

				IplImage* eig_img = cvCreateImage(size, IPL_DEPTH_8U, 1);
				IplImage* temp_img = cvCreateImage(size, IPL_DEPTH_8U, 1);
				cvZero(eig_img);
				cvZero(temp_img);
				cvSetImageROI(objImage, kbRegion);
				cvSetImageROI(dstImage, kbRegion);
				cornerCount= 4;
				
				//cvShowImage("test", objImage);
				cvGoodFeaturesToTrack(objImage, eig_img, temp_img, corners, &cornerCount, 0.1, 200, NULL, 7, 0);
				cvFindCornerSubPix (objImage, corners, cornerCount,cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
				if(cornerCount != 4){
					printf("������ �߸��Ǿ����ϴ�. ROI�� �ٽ� �������ּ���\n");
					mode = SET_ROI;
					mouseSelect.width = 0;
					mouseSelect.height = 0;	
					mouseSelect.x = 0;
					mouseSelect.y = 0;
					cvResetImageROI(dstImage);
					cvResetImageROI(objImage);
					continue;
				}
				for(int i = 0; i < cornerCount ; i++)
					cvCircle(dstImage, cvPointFrom32f(corners[i]), 3, CV_RGB(255,0,0), 3);
				cvResetImageROI(objImage);
				cvResetImageROI(dstImage);
				mode = CORNER_CONFIRM;	
				printf("ǥ�õ� ���� ���� ������ ������ �½��ϱ�?\n ������ ����, �ƴϸ� ������ ��ư���� �巹�� �Ͽ� ������ ���߼���\n");
			}
			else if(mode == CORNER_CONFIRM || mode == ADJUST_CORNER){
				cvSetImageROI(dstImage, kbRegion);
				for(int i = 0; i < cornerCount ; i++)
					cvCircle(dstImage, cvPointFrom32f(corners[i]), 3, CV_RGB(255,0,0), 3);
				cvResetImageROI(dstImage);
				if(mode == ADJUST_CORNER && bRButtonDown && mouseSelect.width > 0 && mouseSelect.height > 0){
					cvSetImageROI(dstImage,mouseSelect);
					cvXorS(dstImage, cvScalarAll(255), dstImage, 0);
					cvResetImageROI(dstImage);
				}
			}
			else if(mode == CATCH_CORNER){
				cvSetImageROI(dstImage, kbRegion);
				for(int i = 0; i < cornerCount ; i++)
					cvCircle(dstImage, cvPointFrom32f(corners[i]), 3, CV_RGB(255,0,0), 3);
				cvResetImageROI(dstImage);
				cvRectangleR(dstImage, mouseSelect, CV_RGB(0,0,255), 2);
			}
			else if(mode == CORNER_MOVE){
				cvSetImageROI(dstImage, kbRegion);
				for(int i = 0; i < cornerCount ; i++)
					cvCircle(dstImage, cvPointFrom32f(corners[i]), 3, CV_RGB(255,0,0), 3);
				cvResetImageROI(dstImage);

			}
			else if(mode == CALC_SPOT){
				IplImage* op = cvCreateImage(cvGetSize(dstImage), IPL_DEPTH_8U, 3);
				cvCopy(dstImage, op);
				cvSetImageROI(dstImage, kbRegion);
				cvSetImageROI(op, kbRegion);
				
				cornerSorting();

				corners[1].y = corners[0].y;
				corners[2].y = corners[3].y;
				srcQuad[Direction_1] = corners[1];
				srcQuad[Direction_5] = corners[2];
				srcQuad[Direction_7] = corners[3];
				srcQuad[Direction_11] = corners[0];

				dstQuad[Direction_1].x = widthROI;
				dstQuad[Direction_1].y = 0;
				dstQuad[Direction_5].x = widthROI;
				dstQuad[Direction_5].y = heightROI;
				dstQuad[Direction_7].x = 0;
				dstQuad[Direction_7].y = heightROI;
				dstQuad[Direction_11].x = 0;
				dstQuad[Direction_11].y = 0;	
				
				
				cvGetPerspectiveTransform(srcQuad, dstQuad, warp_matrix);
				cvWarpPerspective(dstImage, trans, warp_matrix);
				rowSorting(trans, kbRegion, keyCorners);
						

				cvInvert(warp_matrix, warp_matrix_invert);
				
				dst = cvCreateMat(3, 1,CV_32FC1);
				for(int i = 0 ; i < 320 ; i++){
					cvmSet(dst, 0, 0, keyCorners[i].x);  //790 0131 2241 467
					cvmSet(dst, 1, 0, keyCorners[i].y);
					cvmSet(dst, 2, 0, 1);

					cvMatMul(warp_matrix_invert, dst, result);
					float t = cvmGet(result, 2,0);
					keyCorners[i].x = cvmGet(result, 0,0)/t;
					keyCorners[i].y = cvmGet(result, 1,0)/t;

					cvCircle(op, cvPointFrom32f(keyCorners[i]), 1, CV_RGB(255,0,0), 1);
				}
				setkbRegion(keyCorners, 0, 16);
				setkbRegion(keyCorners, 64, 14);
				setkbRegion(keyCorners, 120, 14);
				setkbRegion(keyCorners, 176, 13);
				setkbRegion(keyCorners, 228, 12);
				setkbRegion(keyCorners, 276, 7);
				setkbRegion(keyCorners, 304, 1);
				setkbRegion(keyCorners, 308, 3);
				showRectangleData(op);
				
				cvResetImageROI(dstImage);
				cvResetImageROI(op);
				
				cvShowImage("op", op);
				
				mode = CATCH_SKIN_COLOR;
				printf("������� �յ��� ������ ������ �������� �νʽÿ�. �غ� �Ϸ�Ǹ� ���콺 ���� ��ư�� ��������\n");
			}
			else if(mode == CATCH_SKIN_COLOR){
				cvCircle(dstImage,center, 100, CV_RGB(0,255, 0), 3); 
			}
			else if(mode == CALC_HIST || mode == HAND_TRACKING){
				CvRect handROI= cvRect(center.x-70, center.y - 70, 140, 140);
				CvRect handTrack = cvRect(center.x - 70, center.y - 70 , 300, 300);
				cvCvtColor(dstImage, YCrCbImage, CV_BGR2YCrCb);
				cvInRangeS(YCrCbImage, cvScalar(0, 133, 77), cvScalar(255, 173, 127), handMasking);
				cvSplit(YCrCbImage, 0, CrImage, 0 , 0);
				if(mode == CALC_HIST){
					cvSetImageROI(CrImage, handROI);
					cvCalcHist(&CrImage, pHist, 0);
					cvGetMinMaxHistValue(pHist, 0, &max, 0, 0);
					cvScale(pHist->bins, pHist->bins, max? 255/max : 0);			//���� ������ �ִ� �������� histgram ����.
					cvResetImageROI(CrImage);										//���⼭�� histogram�� ������ ��.
					mode = HAND_TRACKING;
					for(int i = 0 ; i < 4 ; i++)
						buffer[i] = cvCreateImage(size, IPL_DEPTH_8U, 1);
				}
				else{														
					/*cvCalcBackProject(&CrImage, backProject, pHist);				//Histogram�� ���� ������ �̹����� ��´�.
					cvAnd(backProject, handMasking, backProject);

					cvDilate(backProject, backProject, 0, 3);
					cvErode(backProject, backProject, 0, 3);
					cvAndS(backProject, cvScalar(180, 180, 180), backProject);

					cvMeanShift(backProject, handTrack, criteria, &handTracking);
					handTrack = handTracking.rect;*/
					
					CvRect objKey = kbRegionInfo[2][10];
					objKey.x = kbRegionInfo[2][10].x + kbRegion.x;
					objKey.y = kbRegionInfo[2][10].y + kbRegion.y;
					
					cvCvtColor(dstImage, buffer[last], CV_BGR2GRAY);
					for(int i = 0 ; i < 4 ; i++)
						cvSetImageROI(buffer[i], objKey);
					cvSetImageROI(silh, objKey);
					cvSetImageROI(mhi, objKey);
					cvSetImageROI(moveMask, objKey);
					cvSetImageROI(segmask, objKey);
					cvSetImageROI(orient, objKey);

					//cvSetImageROI(dstImage, kbRegion);
					curr = last;
					prev = (curr+1) %4;
					last = prev;
					silh = buffer[prev];

					cvAbsDiff(buffer[prev], buffer[curr], silh);
					cvThreshold(silh, silh, threshold, 255, 0);
					timeStamp = (double) clock()/ CLOCKS_PER_SEC;

					cvUpdateMotionHistory(silh, mhi, timeStamp, MHI_DURATION);
					double scale = 255./MHI_DURATION;
					double t = MHI_DURATION - timeStamp;

					cvScale(mhi, moveMask, scale, t*scale);
					
					cvCalcMotionGradient(mhi, moveMask, orient, MAX_TIME_DELTA, MIN_TIME_DELTA, 3);
					cvClearMemStorage(storage);

					seq = cvSegmentMotion(mhi, segmask, storage, timeStamp, MAX_TIME_DELTA);
					
					
					if(seq->total > 0)
						DrawMotionOrientation(seq, silh, mhi, orient,	moveMask, dstImage, timeStamp,objKey);
					for(int i = 0 ; i < 4 ; i++)
						cvResetImageROI(buffer[i]);
					cvResetImageROI(silh);
					cvResetImageROI(mhi);
					cvResetImageROI(moveMask);
					cvResetImageROI(segmask);
					cvResetImageROI(orient);
					//cvResetImageROI(dstImage);
				}
			}
		}
		if(mode > ROI_CONFIRM && mode < CATCH_SKIN_COLOR)
			cvRectangleR(dstImage, kbRegion, CV_RGB(0, 255, 0), 2);
		//cvShowImage("silh", silh);
		cvShowImage("FRAME", dstImage);
		//cvShowImage("motion", silh);
		char key = cvWaitKey(30);
		if(key == 27)
			break;
	}
}