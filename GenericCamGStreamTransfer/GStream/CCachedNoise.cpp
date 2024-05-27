#include "CCachedNoise.h"

#include "CCachedNoise.h"

void CCachedNoise::Init(ENoiseType noise_type, int width, int height, int cnt, float mean, float std_dev)
{
    NoiseImages.clear();
    NoiseImages.reserve(cnt);

    std::random_device rd;
    generator = std::default_random_engine(rd());
    distribution = std::uniform_int_distribution<int>(0, cnt - 1);

    for (int i = 0; i < cnt; ++i)
    {
        cv::Mat noiseImage(height, width, CV_8UC4); // Use 8-bit 4-channel

        if (noise_type == ENoiseType::Gauss)
        {
            cv::Mat tempNoise(height, width, CV_32FC4);
            cv::randn(tempNoise, cv::Scalar(mean, mean, mean, mean), cv::Scalar(std_dev, std_dev, std_dev, std_dev)); // Gaussian noise with mean 128 and stddev 25
            tempNoise.convertTo(noiseImage, CV_8UC4); // Convert to 8-bit 4-channel
        }
        else if (noise_type == ENoiseType::Uniform)
        {
            cv::randu(noiseImage, cv::Scalar(0, 0, 0, 0), cv::Scalar(255, 255, 255, 255)); // Uniform noise between 0 and 255
        }

        NoiseImages.push_back(noiseImage);
    }
}

cv::Mat* CCachedNoise::GetRandom()
{
    int index = distribution(generator);
    return &NoiseImages[index];
}

void CCachedNoise::ApplyBlurFilter(const cv::Mat& src, cv::Mat& dst, int blurAmount)
{
    int kernelSize = blurAmount; // Use blurAmount as kernel size
    int border = kernelSize / 2;
    auto size = src.size();

    int numPixels = kernelSize * kernelSize;
    float one_over_num_pixels = 1.0f / numPixels;

    dst = cv::Mat::zeros(src.size(), src.type());

    for (int y = border; y < src.rows - border; ++y)
    {
        int sumB = 0, sumG = 0, sumR = 0, sumA = 0;

        // Initialize sum for the first window
        for (int ky = -border; ky <= border; ++ky)
        {
            for (int kx = -border; kx <= border; ++kx)
            {
                _int64 addr = (y + ky) * size.width + border + kx;
                uchar* p_data = &src.data[addr * 4];

                sumB += p_data[0];
                sumG += p_data[1];
                sumR += p_data[2];
                sumA += p_data[3];
            }
        }

        // Slide the window across the row
        for (int x = border; x < src.cols - border; ++x)
        {
            if (x > border)
            {
                // Subtract the leftmost column
                for (int ky = -border; ky <= border; ++ky)
                {
                    _int64 addr = (y + ky) * size.width + x - border - 1;
                    uchar* p_data = &src.data[addr * 4];

                    sumB -= p_data[0];
                    sumG -= p_data[1];
                    sumR -= p_data[2];
                    sumA -= p_data[3];
                }

                // Add the rightmost column
                for (int ky = -border; ky <= border; ++ky)
                {
                    _int64 addr = (y + ky) * size.width + x + border;
                    uchar* p_data = &src.data[addr * 4];

                    sumB += p_data[0];
                    sumG += p_data[1];
                    sumR += p_data[2];
                    sumA += p_data[3];
                }
            }

            uchar* p_dest_addr = &dst.data[(y * size.width + x) * 4];
            p_dest_addr[0] = static_cast<uchar>(sumB * one_over_num_pixels);
            p_dest_addr[1] = static_cast<uchar>(sumG * one_over_num_pixels);
            p_dest_addr[2] = static_cast<uchar>(sumR * one_over_num_pixels);
            p_dest_addr[3] = static_cast<uchar>(sumA * one_over_num_pixels);
        }
    }
}