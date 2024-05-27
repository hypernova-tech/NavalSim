#pragma once
#include <gst/gst.h>
using namespace std;
#include <string>
#include <vector>
#include <thread>
#include "CSharedMem.h"
#include <opencv2/opencv.hpp>
#include "CCachedNoise.h"

class GStreamSender;
struct SUserData
{
	GstElement* pipeline, * appsrc, * conv, * payloader, * sink;
	GStreamSender* pSender;
};
class GStreamSender
{

protected:
	GstElement* AppSrc;
	
	uint8_t* pOut;
	void FeedData(uint8_t* data, int size);
	std::thread* pThread;

	void SenderLoop();
	
	SUserData UserData;
	GstBuffer* pFeederBuffer = nullptr;
	cv::Mat* pimage;
	CCachedNoise CachedNoise;
public:
	int TestImageBufferSize = 0;
	uint8_t* pTestImageBuffer;
	uint8_t* pOutputBuffer;
	GMainLoop* pLoop;
	GstClockTime TimeStamp;
	bool Init(string name, string ip_addr, int port, int width, int height, int frame_rate);
	void OnReceivedFrame(SSharedMemBufferHdr *p_hdr, const std::uint8_t* p_buffer, size_t width, size_t height);
};

