#include <iostream>
#include "wiringPi.h"
#include "config.h"
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "electronics.hpp"
#include <vector>

using namespace std;
using namespace cv;

#define MOVEMENT_THRESHOLD 800

void Shutdown()
{
	SetOk(false);
}

int main()
{
	wiringPiSetup();
	pinMode(GREEN_LED_PIN,OUTPUT);
	SetOk(true);
	
	std::cout << "Starting security system!" << std::endl;
	
	VideoCapture cap(0);
	if(cap.isOpened() == false )
	{
		std::cout << "failed to get camera" << std::endl;
		Shutdown();
		return 1;
	}
		
	namedWindow("footage",1);
	
	Mat frame,mask,gray;
	BackgroundSubtractorMOG2 bs;
	
	vector<vector<Point>> contours;
	vector<Vec4i> heir;
	
	int count = 0;
	bool didMove = false;
	
	for(;;)
	{
		cap >> frame;
		cvtColor(frame,gray,COLOR_RGB2GRAY);
		bs(gray,mask,0.1);

		dilate(mask,gray,NULL);
		findContours(gray,contours,heir,RETR_TREE,CHAIN_APPROX_SIMPLE,Point(0,0));
		
		int total = 0;
		
		for(int i = 0; i < contours.size(); i++)
		{
			double area = contourArea(contours[i]);
			total += area;
		}
		
		//checks for movement
		if(total > MOVEMENT_THRESHOLD)
		{
			//did we move last frame
			if(!didMove)
			{
				didMove = true;
				count += 1;
				cout << count << endl;
			}
		}else{
			//we didnt move so reset
			didMove = false;
		}
		
		
		
		imshow("footage",frame);
		if(waitKey(30) >= 0)
			break;
	}
	
	Shutdown();
	return 0;
}