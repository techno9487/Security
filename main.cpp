#include <iostream>
#include "wiringPi.h"
#include "config.h"
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "electronics.hpp"
#include <vector>
#include "NotificationCentre.hpp"

using namespace std;
using namespace cv;

#define MOVEMENT_THRESHOLD 800

void Shutdown()
{
	SetOk(false);
}

void Movement(bool isNow);

NotificationCentre centre;

int main()
{
	wiringPiSetup();
	pinMode(GREEN_LED_PIN,OUTPUT);
	pinMode(BUZZER_PIN,OUTPUT);
	pinMode(LDR_PIN,INPUT);
	pinMode(RED_LED_PIN,OUTPUT);
	SetOk(true);
	centre.Run();
	
	std::cout << "Starting security system!" << std::endl;
	
	VideoCapture cap(0);
	if(cap.isOpened() == false )
	{
		std::cout << "failed to get camera" << std::endl;
		Shutdown();
		return 1;
	}
		
	namedWindow("footage",1);
	namedWindow("live",2);
	
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
				Movement(true);
				imwrite("/tmp/photo.jpg",frane);
			}
		}else{
			//we didnt move so reset
			didMove = false;
			Movement(false);
		}
		
		imshow("footage",gray);
		imshow("live",frame);
		if(waitKey(30) >= 0)
			break;
	}
	
	Shutdown();
	return 0;
}

void Movement(bool isNow)
{
	SetBuzzer(isNow);
	SetAlarm(isNow);

	if(isNow)
	{
		struct Notification note;
		note.info = "/tmp/photo.jpg";
		note.type = IMAGE;

		centre.SubmitJob(note);
		//TODO: Generate uuid save image with uuid as name then submit
		//to messenger process to forward it onto facebook
	}
}
