#include"Header.h"

void cornerIDentified(CvPoint2D32f* corners){
	int index = 0;
	CvPoint2D32f pt[4]; 

	for(int i = 0 ; i < 4 ; i++)
		pt[i] = cvPoint2D32f((corners+0)->x, (corners+0)->y);

	for(int i = 0 ; i < 321 ; i++){
		if(pt[0].x >= (corners + i)->x){
			pt[0].x = (corners + i)->x;
			pt[0].y = (corners + i)->y;
		}
	}

	printf("index = %d\n", index);
}