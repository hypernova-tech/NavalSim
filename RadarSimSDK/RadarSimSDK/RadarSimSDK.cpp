// RadarSimSDK.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "RadarSimSdkConfig.h"
#if SIMULATE_HOST > 0

#include <iostream>
#include "CHost.h"
#include <opencv2/opencv.hpp>

#include <PPIController.h>



CHost* pHost;







int main()
{
#if 0



		cv::Mat Image;
		Image = cv::Mat::zeros(1024, 1024, CV_8UC4);

		// Draw a circle
		cv::circle(Image, cv::Point(320, 240), 50, cv::Scalar(0, 0, 255), -1);

		// Create a window
		cv::namedWindow("Window", cv::WINDOW_AUTOSIZE);

		// Show our image inside it
		cv::imshow("Window", Image);

    // Wait for a keystroke in the window
    //



		int n;
		while (true) {
			cv::waitKey(25);
		}

#endif

    tMultiRadarClient::GetInstance()->Connect();

    
    while (true) {
		int n;
		std::cin >> n;
    }

    std::cout << "Hello World!\n";
}

#endif

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
