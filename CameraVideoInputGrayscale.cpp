
// the code takes a video input and captures frames at an interval of 33 sec and when the user presses "escape" key, it exits.

#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <math.h>
//#include "BlobResult.h"


int BWThresh_pos=0;
IplImage* func_frame = NULL;

void BWConvert(int pos){

	IplImage *gframe=NULL, *bframe=NULL;
	
	gframe=cvCreateImage(cvSize(func_frame->width,func_frame->height),8,1);
	bframe=cvCreateImage(cvSize(func_frame->width,func_frame->height),8,1);

	cvCvtColor(func_frame,gframe,CV_BGR2GRAY);
	cvThreshold( gframe, bframe, pos, 255, CV_THRESH_BINARY );
	cvShowImage("Example2",bframe);
	char c = cvWaitKey(15);
	if( c == 27 ) 
	{
			return;

	}

}

void HSVConvert(){
}

void HSVSeperation(){
}


int _tmain(int argc, _TCHAR* argv[])
{
	//cvNamedWindow( "Example1", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE );
	/*cvNamedWindow( "Example3", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Example4", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Example5", CV_WINDOW_AUTOSIZE );*/
	//CvCapture* capture = cvCreateFileCapture( "vid.avi" );
	CvCapture* capture = cvCreateCameraCapture(0);
	CvSize size;
	int depth=0;
	IplImage* frame;
	IplImage* gframe=NULL;
	IplImage* hsvframe=NULL;
	IplImage* bframe=NULL;
    IplImage* contourFrame=NULL;
	CvMemStorage* 	g_storage = NULL;
	CvSeq* contours = 0;

	while(1) {
		frame = cvQueryFrame( capture);
		gframe=frame;

		/*func_frame=frame;*/

		if( !frame ) break;
		
		IplImage* contourFrame = cvCreateImage( cvGetSize(frame), 8, 1);
		g_storage = cvCreateMemStorage(0);
		IplImage* hsv = cvCreateImage( cvGetSize(frame), 8, 3); //full hsv image 
		IplImage* hue = cvCreateImage( cvGetSize(frame), 8, 1);	// Hue component.
		IplImage* sat = cvCreateImage( cvGetSize(frame), 8, 1);	// Saturation component.
		IplImage* val = cvCreateImage( cvGetSize(frame), 8, 1);	// Brightness component.
/*
		func_frame = cvCreateImage( cvGetSize(frame), 8, 3);
		func_frame=frame;
    */
			cvZero(hue);
			cvZero(sat);
			cvZero(val);
			cvZero(hsv);

		hsvframe=cvCreateImage(cvSize(frame->width,frame->height),8,3);
		gframe=cvCreateImage(cvSize(frame->width,frame->height),8,1);
		bframe=cvCreateImage(cvSize(frame->width,frame->height),8,1);
		
		cvCvtColor(frame,gframe,CV_BGR2GRAY);
		cvCvtColor(frame,hsvframe,CV_RGB2HSV);

		cvSplit(hsvframe,hue,sat,val,0);
		
		cvThreshold( gframe, bframe, 100, 255, CV_THRESH_BINARY );
		
		cvThreshold(hue, hue, 29, 180, CV_THRESH_BINARY);
		cvThreshold(sat, sat,30, 180, CV_THRESH_BINARY);
		cvThreshold(val, val, 60, 180, CV_THRESH_BINARY);

		//cvCreateTrackbar("Threshold","Example2",&BWThresh_pos,200,BWConvert);
		
		IplImage* SkinDetectedFrame = cvCreateImage( cvGetSize(frame), 8, 1);
		cvAnd(hue, sat, SkinDetectedFrame);				
		cvAnd(SkinDetectedFrame, val, SkinDetectedFrame);	
		
		
		cvErode(SkinDetectedFrame,SkinDetectedFrame,NULL,1);
		cvDilate(SkinDetectedFrame,SkinDetectedFrame,NULL,1);

		cvSmooth(SkinDetectedFrame,SkinDetectedFrame,CV_BLUR,3,3,0,0);


		//cvErode
			
		contourFrame=bframe;
		
		/* contour using cvFindContour*/
						
		/*contours=0;
		cvFindContours( contourFrame, g_storage, &contours );
		cvConvexHull2(contours, g_storage, CV_CLOCKWISE);

		if( contours ){
		cvDrawContours(contourFrame, contours,cvScalarAll(255),cvScalarAll(255),255,3,4 );
	    
		}

	   cvShowImage( "Example2", contourFrame );
		*/


		/*cvShowImage( "Example1", hsvframe);*/
		cvShowImage( "Example2", SkinDetectedFrame);
		/*cvShowImage( "Example3", hue);
		cvShowImage( "Example4", sat);
		cvShowImage( "Example5", gframe);*/
		char c = cvWaitKey(33);
		if( c == 27 ) break;
		frame=NULL;
		gframe=bframe=hsvframe=hue=sat=val=SkinDetectedFrame=NULL;
	}
	cvReleaseCapture( &capture );
	/*cvDestroyWindow( "Example1" );*/
	cvDestroyWindow( "Example2" );
	/*cvDestroyWindow( "Example3" );
	cvDestroyWindow( "Example4" );
	cvDestroyWindow( "Example5" );*/
	return 0;
}

