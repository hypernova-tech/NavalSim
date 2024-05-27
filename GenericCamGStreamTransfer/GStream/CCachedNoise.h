#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <random>
using namespace std;

enum ENoiseType
{
	Gauss = 0,
	Uniform
};

struct SNoiseChannel
{
	float x;
	float y;
	float z;
};

class CCachedNoise
{
public:
	void Init(ENoiseType noise_type, int width, int height, int cnt, float mean, float std_dev);
	cv::Mat* GetRandom();
	void ApplyBlurFilter(const cv::Mat& src, cv::Mat& dst, int blurAmount);

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
	vector<cv::Mat> NoiseImages; // RGB channels
};

