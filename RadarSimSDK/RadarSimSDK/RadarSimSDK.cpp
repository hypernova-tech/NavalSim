// RadarSimSDK.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CHost.h"
#include "RadarSimSdkConfig.h"

CHost* pHost;

#if SIMULATE_HOST > 0
#include <opencv2/opencv.hpp>
cv::Mat* pimage;


int main()
{
    pimage = new cv::Mat(cv::Mat::zeros(1024, 1024, CV_8UC4));
    tMultiRadarClient::GetInstance()->Connect();

    unsigned int * p_data = (unsigned int*) pimage->data;
 

    // Wait for a key press indefinitely or for a specific amount of time (0 means indefinitely)
    //

    
    while (true) {
        cv::waitKey(1);
        cv::imshow("Display window", *pimage);
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
