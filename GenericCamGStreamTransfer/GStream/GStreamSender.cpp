#include "GStreamSender.h"
#include <iostream>
#include "CMath.h"
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include "CStreamManager.h"
#include <random>
using namespace std;

uint8_t* pLastBuff;
int LastSize;
int TotalPushed = 0;

static void enough_data_callback(GstAppSrc* src, gpointer user_data) {
    // Your code to handle the situation when appsrc has enough data
    //std::cout << "appsrc has enough data, pausing or slowing data feed." << std::endl;
    // You can use user_data to access your application's data structures
}
static void need_data_callback(GstAppSrc* src, guint length, gpointer user_data) {


    return;
    SUserData* p_data = (SUserData*)user_data;
    static gboolean white = FALSE;
    GstBuffer* buffer;
    guint size;
    GstFlowReturn ret;
    static uint8_t val = 0;

    // Generate or load your RGB data here
    // For demonstration, we'll just create a simple alternating white/black frame
    size = 720 * 576 * 3; // Width * Height * RGB
    buffer = gst_buffer_new_allocate(NULL, size, NULL);

    memset(p_data->pSender->pTestImageBuffer, val, p_data->pSender->TestImageBufferSize);
    val++;

    // Fill the buffer with black/white for demonstration
    gst_buffer_fill(buffer, 0, p_data->pSender->pTestImageBuffer, size);
    //white = !white;

    // Push the buffer into the appsrc
    g_signal_emit_by_name(p_data->appsrc, "push-buffer", buffer, &ret);

    // Free the buffer after pushing
    if (ret != GST_FLOW_OK) {
        // Error handling
    }

    ///gst_buffer_unref(buffer);
}

bool GStreamSender::Init(string name, string ip_addr, int port, int width, int height, int frame_rate)
{


#if 1

    CachedNoise.Init(ENoiseType::Gauss, width, height, 8, 28, 14);

    pimage = new cv::Mat(cv::Mat::zeros(height, width, CV_8UC4));

    cv::namedWindow("ImVis", cv::WINDOW_NORMAL);
    cv::resizeWindow("ImVis", 800, 600);

    GstElement* pipeline, * appsrc, * videoconvert, * x264enc, * rtph264pay, * udpsink;
    GstBuffer* buffer;
    GstFlowReturn ret;
    GstMapInfo map;
    guint8* raw_data;
    gsize raw_data_size = 0;

    // Initialize GStreamer
    gst_init(NULL, NULL);
    pLoop = g_main_loop_new(NULL, FALSE);

    // Create the elements
    appsrc = gst_element_factory_make("appsrc", "mysource");
    videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    x264enc = gst_element_factory_make("x264enc", "x264enc");
    rtph264pay = gst_element_factory_make("rtph264pay", "rtph264pay");
    udpsink = gst_element_factory_make("udpsink", "udpsink");

    // Check if elements are created successfully
    if (!appsrc || !videoconvert || !x264enc || !rtph264pay || !udpsink) {
        std::cerr << "Failed to create one or more elements.\n";
        return false;
    }

    // Create the empty pipeline
    pipeline = gst_pipeline_new("my-pipeline");

    if (!pipeline) {
        std::cerr << "Failed to create pipeline.\n";
        return false;
    }

    // Build the pipeline
    gst_bin_add_many(GST_BIN(pipeline), appsrc, videoconvert, x264enc, rtph264pay, udpsink, NULL);
    if (!gst_element_link_many(appsrc, videoconvert, x264enc, rtph264pay, udpsink, NULL)) {
        std::cerr << "Elements could not be linked.\n";
        gst_object_unref(pipeline);
        return false;
    }

    // Configure the elements
    g_object_set(udpsink, "host", ip_addr.c_str(), "port", port, "sync", FALSE, NULL);

    char capstr[1024];
    sprintf_s(capstr, "video/x-raw,format=BGRA,width=%d,height=%d,framerate=%d/1", width, height, frame_rate);

    g_object_set(G_OBJECT(appsrc), "caps", gst_caps_from_string(capstr), "format", GST_FORMAT_TIME, NULL);

    // Correctly set the tune property using a string value
    //g_object_set(G_OBJECT(x264enc), "speed-preset", 1, "tune", "zerolatency", "bitrate", 4096, NULL);

    /*
    stillimage (0x00000001) – Still image
fastdecode (0x00000002) – Fast decode
zerolatency (0x00000004) – Zero latency
*/
    g_object_set(G_OBJECT(x264enc), "tune", 0x00000004, NULL);
    //g_object_set(G_OBJECT(x264enc), "speed-preset", 0x00000001, NULL);
    g_object_set(G_OBJECT(x264enc), "bitrate", 4096*2, NULL);

    // Allocate and initialize buffers
    TestImageBufferSize = width * height * 4;  // BGRA has 4 channels
    pTestImageBuffer = new uint8_t[TestImageBufferSize];
    memset(pTestImageBuffer, 0, TestImageBufferSize);
    pOutputBuffer = new uint8_t[width * height * 4];

    UserData.appsrc = appsrc;
    UserData.pSender = this;

    this->AppSrc = appsrc;
    g_signal_connect(appsrc, "need-data", G_CALLBACK(need_data_callback), &UserData);
    g_signal_connect(appsrc, "enough-data", G_CALLBACK(enough_data_callback), &UserData);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    pThread = new std::thread(&GStreamSender::SenderLoop, this);

    return true;
#endif
}

void GStreamSender::SenderLoop()
{
    while (true) {
        g_main_loop_run(pLoop);
    }
    
}
void addGaussianNoise(cv::Mat* image, double mean, double stddev, double noiseFraction) {
    
#if false
    int numPixels = image->rows * image->cols;
    int numNoisePixels = static_cast<int>(noiseFraction * numPixels);

    // Create a random engine and distributions
    std::default_random_engine generator;
    std::uniform_int_distribution<int> rowDist(0, image->rows - 1);
    std::uniform_int_distribution<int> colDist(0, image->cols - 1);
    std::normal_distribution<double> noiseDist(mean, stddev);

    // Add Gaussian noise to a subset of pixels
    for (int i = 0; i < numNoisePixels; ++i) {
        int row = rowDist(generator);
        int col = colDist(generator);
        for (int c = 0; c < image->channels()-1; ++c) {
            int noise = static_cast<int>(noiseDist(generator));
            int newValue = image->at<cv::Vec3b>(row, col)[c] + noise;
            newValue = std::min(std::max(newValue, 0), 255); // Clamp the value between 0 and 255
            image->at<cv::Vec3b>(row, col)[c] = static_cast<uchar>(newValue);
        }
    }
#endif

    

}

// Function to add barrel distortion to an image
cv::Mat addBarrelDistortion(const cv::Mat*image, double k1, double k2, double k3) {
    cv::Mat distortedImage;
    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 1, 0, image->cols / 2, 0, 1, image->rows / 2, 0, 0, 1);
    cv::Mat distCoeffs = (cv::Mat_<double>(1, 5) << k1, k2, 0, 0, k3);

    cv::undistort(*image, distortedImage, cameraMatrix, distCoeffs);

    return distortedImage;
}



void GStreamSender::OnReceivedFrame(SSharedMemBufferHdr *p_hdr, const std::uint8_t* p_buffer, size_t width, size_t height)
{

  
    pimage->data = (uchar*)p_buffer;
    cv::Mat inputImage(height, width, CV_8UC4, (void*)p_buffer);
    
    //memcpy(pimage->data, p_buffer, width * height * 4);
    //cv::Mat blurredImage;
    //cv::blur(inputImage, blurredImage, cv::Size(3, 3)); // Kernel size of 15x15

    auto p_noise = CachedNoise.GetRandom();
    inputImage += *p_noise;
    cv::Mat blured(height, width, CV_8UC4, (void*)p_buffer);
   // CachedNoise.ApplyBlurFilter(inputImage, blured, 5);
    auto noise_start = std::chrono::high_resolution_clock::now(); // Time after blur
    {
        int blurAmount = 5;
        int kernelSize = blurAmount; // Use blurAmount as kernel size
        int border = kernelSize / 2;
        auto size = inputImage.size();

        int numPixels = kernelSize * kernelSize;
        float one_over_num_pixels = 1.0 / numPixels;

        for (int y = border; y < inputImage.rows - border; ++y)
        {
            for (int x = border; x < inputImage.cols - border; ++x)
            {
                int sumB = 0, sumG = 0, sumR = 0, sumA = 0;
                for (int ky = -border; ky <= border; ++ky)
                {
                    for (int kx = -border; kx <= border; ++kx)
                    {
                        _int64 addr = (y + ky) * size.width + x + kx;
                        uchar* p_data = &inputImage.data[addr * 4];

                        sumB += p_data[0];
                        sumG += p_data[1];
                        sumR += p_data[2];
                        sumA += p_data[3];
                    }
                }

                uchar* p_dest_addr = &blured.data[((y)*size.width + x) * 4];
                p_dest_addr[0] = (sumB * one_over_num_pixels);
                p_dest_addr[1] = (sumG * one_over_num_pixels);
                p_dest_addr[2] = (sumR * one_over_num_pixels);
                p_dest_addr[3] = (sumA * one_over_num_pixels);
            }
        }
    }

   
    //cv::Mat noise(pimage->size(), pimage->type());
    //cv::randn(noise, 20, 10); // Generate Gaussian noise
    //*pimage += noise; // Add the noise to the original image
   // auto distorted = addBarrelDistortion(pimage, -1e-15, 1e-13, 0.0);
    auto noise_end = std::chrono::high_resolution_clock::now(); // Time after blur
    std::chrono::duration<double> noise_duration = noise_end - noise_start;
    std::cout << "noise_duration: " << noise_duration.count() << " seconds" << std::endl;

    cv::imshow("ImVis", blured);
    FeedData((uint8_t * )pimage->data, width * height * 4);
}





void GStreamSender::FeedData(uint8_t* data, int size) {


    if (!this->AppSrc) {
        return; // AppSrc is not initialized
    }



    GstClockTime buffer_duration = gst_util_uint64_scale_int(1, GST_SECOND, 60); // 1/30th of a second
   
    GstMapInfo map;
    /*
    if (pFeederBuffer == nullptr) {
        pFeederBuffer = gst_buffer_new_allocate(NULL, size, NULL);
        pFeederBuffer = gst_buffer_make_writable(pFeederBuffer);
  
    }
    */
    pFeederBuffer = gst_buffer_new_allocate(NULL, size, NULL);
    /*
    if (gst_buffer_map(pFeederBuffer, &map, GST_MAP_WRITE)) {
        memcpy(map.data, data, size);
        gst_buffer_unmap(pFeederBuffer, &map);
    }
    */
    gst_buffer_fill(pFeederBuffer, 0, data, size);
    
    //auto filled = gst_buffer_fill(pFeederBuffer, 0, data, size);
    // Set buffer PTS. DTS is not set here as it's often not necessary for simple cases.
    GST_BUFFER_PTS(pFeederBuffer) = TimeStamp;
    GST_BUFFER_DURATION(pFeederBuffer) = buffer_duration;

    TimeStamp += buffer_duration;
    GstFlowReturn ret;
   

    g_signal_emit_by_name(this->AppSrc, "push-buffer", pFeederBuffer, &ret);

    if (ret != GST_FLOW_OK) {
        // Handle error
    }
    
    gst_buffer_unref(pFeederBuffer);

   
   
    cv::waitKey(1);

    //g_timer_start(this->AppSrc->timer);
}