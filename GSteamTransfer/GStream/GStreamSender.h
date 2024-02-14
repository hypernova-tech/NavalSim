#pragma once
#include <gst/gst.h>
using namespace std;
#include <string>
#include <vector>
#include <thread>


class GStreamSender
{

protected:
	GstElement* AppSrc;
	GMainLoop* pLoop;
	uint8_t* pOut;
	uint8_t* ConvertRGBAtoYCbCr(const std::uint8_t* p_buffer, size_t width, size_t height);
	void FeedData(uint8_t* data, int size);
	std::thread* pThread;

	void SenderLoop();
public:
	bool Init(string name, string ip_addr, int port, int width, int height, int frame_rate);
	void OnReceivedFrame(const std::uint8_t* p_buffer, size_t width, size_t height);
};

