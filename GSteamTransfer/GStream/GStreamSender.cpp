#include "GStreamSender.h"
#include <iostream>
#include "CMath.h"
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include "CStreamManager.h"
using namespace std;

uint8_t* pLastBuff;
int LastSize;
int TotalPushed = 0;

static void enough_data_callback(GstAppSrc* src, gpointer user_data) {
    // Your code to handle the situation when appsrc has enough data
    std::cout << "appsrc has enough data, pausing or slowing data feed." << std::endl;
    // You can use user_data to access your application's data structures
}
static void need_data_callback(GstAppSrc* src, guint length, gpointer user_data) {

#if false
    // Your code to provide data to appsrc
   
    
    SUserData* p_data = (SUserData *) user_data;
    TotalPushed += length;
    if (TotalPushed >= p_data->pSender->TestImageBufferSize) {
        TotalPushed = TotalPushed;
    }
    //GstFlowReturn ret;
    //g_signal_emit_by_name(src, "push-buffer", nullptr, &ret);

    GstClockTime buffer_duration = gst_util_uint64_scale_int(1, GST_SECOND, 30); // 1/30th of a second

    GstBuffer* buffer = gst_buffer_new_allocate(NULL, p_data->pSender->TestImageBufferSize, NULL);

    auto filled = gst_buffer_fill(buffer, 0, p_data->pSender->pTestImageBuffer, p_data->pSender->TestImageBufferSize);
    // Set buffer PTS. DTS is not set here as it's often not necessary for simple cases.
    GST_BUFFER_PTS(buffer) = p_data->pSender->TimeStamp;
    GST_BUFFER_DTS(buffer) = p_data->pSender->TimeStamp;
    GST_BUFFER_DURATION(buffer) = buffer_duration;

    p_data->pSender->TimeStamp += buffer_duration;
    GstFlowReturn ret;


    g_signal_emit_by_name(p_data->appsrc, "push-buffer", buffer, &ret);

    if (ret != GST_FLOW_OK) {
        // Handle error
    }

    

    gst_buffer_unref(buffer);

#endif

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
#if 0
    GstElement* pipeline, * appsrc, * conv, * payloader, * sink;
  

   
    // Initialize GStreamer
    gst_init(NULL, NULL);

    // Create main event loop
    pLoop = g_main_loop_new(NULL, TRUE);

    // Create pipeline
    pipeline = gst_pipeline_new(name.c_str());

    // Create elements
    appsrc = gst_element_factory_make("appsrc", NULL);
    conv = gst_element_factory_make("videoconvert", NULL);
    payloader = gst_element_factory_make("rtpvrawpay", NULL);
    sink = gst_element_factory_make("udpsink", NULL);

    if (!pipeline || !appsrc || !conv || !payloader || !sink) {
        g_printerr("Not all elements could be created.\n");
        return false;
    }

    // Set appsrc properties and caps
    GstCaps* caps = gst_caps_new_simple("video/x-raw",
        "format", G_TYPE_STRING,  "RGB",//"YCbCr-4:2:2",
        "width", G_TYPE_INT, width,
        "height", G_TYPE_INT, height,
        "framerate", GST_TYPE_FRACTION, frame_rate, 1,
        NULL);
    g_object_set(G_OBJECT(appsrc), "caps", caps, "format", GST_FORMAT_TIME, NULL);
    TestImageBufferSize = width * height * 3;
    pTestImageBuffer = new uint8_t[TestImageBufferSize];
    memset(pTestImageBuffer, 0, TestImageBufferSize);

    //guint min_buffers_percent = 10; // Example value: 10%
    //g_object_set(G_OBJECT(appsrc), "min-percent-buffers", min_buffers_percent, NULL);


    UserData.appsrc = appsrc;
    UserData.pSender = this;
    
  
   

    // Set sink properties
    g_object_set(sink, "host", ip_addr.c_str(), "port", port, NULL); // Adjust IP and port as needed

    // Build the pipeline
    gst_bin_add_many(GST_BIN(pipeline), appsrc, conv, payloader, sink, NULL);
    
    if (gst_element_link_many(appsrc, conv, payloader, sink, NULL) != TRUE) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return false;
    }

    // Store the appsrc for later use
    this->AppSrc = (appsrc); // Ensure AppSrc is a member of GStreamSender of type GstAppSrc*
    g_signal_connect(appsrc, "need-data", G_CALLBACK(need_data_callback), &UserData);
    g_signal_connect(appsrc, "enough-data", G_CALLBACK(enough_data_callback), &UserData);
    // Start playing
    auto playret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // You would typically not run the main loop here in a real application
    gst_caps_unref(caps);

    // Instead of running the loop here, you might want to run it in a separate thread or use async mechanisms
    pThread = new std::thread(&GStreamSender::SenderLoop, this);

    return true;

#endif

#if 0
    GstElement* pipeline, * appsrc, * videoconvert, * videoscale, * capsfilter, * udpsink;
    GstBus* bus;
    GstMessage* msg;
    GstCaps* caps;

    // Initialize GStreamer
    gst_init(NULL, NULL);

    // Create main event loop
    pLoop = g_main_loop_new(NULL, FALSE);

    // Create the elements
    appsrc = gst_element_factory_make("appsrc", "source");
    videoconvert = gst_element_factory_make("videoconvert", "converter");
    videoscale = gst_element_factory_make("videoscale", "scaler");
    capsfilter = gst_element_factory_make("capsfilter", "filter");
    udpsink = gst_element_factory_make("udpsink", "udp-output");

    // Create the empty pipeline
    pipeline = gst_pipeline_new("test-pipeline");

    if (!pipeline || !appsrc || !videoconvert || !videoscale || !capsfilter || !udpsink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    // Build the pipeline
    gst_bin_add_many(GST_BIN(pipeline), appsrc, videoconvert, videoscale, capsfilter, udpsink, NULL);
    if (gst_element_link_many(appsrc, videoconvert, videoscale, capsfilter, udpsink, NULL) != TRUE) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    // Configure appsrc
    caps = gst_caps_new_simple("video/x-raw",
        "format", G_TYPE_STRING, "RGB",
        "width", G_TYPE_INT, width,
        "height", G_TYPE_INT, height,
        "framerate", GST_TYPE_FRACTION, 30, 1,
        NULL);
    g_object_set(G_OBJECT(appsrc), "caps", caps, NULL);
    g_object_set(appsrc, "do-timestamp", TRUE, "caps", caps,
        "format", GST_FORMAT_TIME, NULL);
   // g_object_set(G_OBJECT(appsrc), "max-bytes", (guint64)4096*4096, NULL);
    gst_caps_unref(caps);

    // Configure capsfilter for the desired YCbCr 4:2:2 format
    caps = gst_caps_new_simple("video/x-raw",
        "format", G_TYPE_STRING, "RGB", // or use "UYVY" depending on your need
        NULL);
    g_object_set(G_OBJECT(capsfilter), "caps", caps, NULL);
    gst_caps_unref(caps);

    // Configure udpsink
    g_object_set(G_OBJECT(udpsink),
        "host", ip_addr,
        "port", port,
        NULL);

    TestImageBufferSize = width * height * 3;
    pTestImageBuffer = new uint8_t[TestImageBufferSize];
    memset(pTestImageBuffer, 0, TestImageBufferSize);

    UserData.appsrc = appsrc;
    UserData.pSender = this;

    this->AppSrc = (appsrc); // Ensure AppSrc is a member of GStreamSender of type GstAppSrc*
    g_signal_connect(appsrc, "need-data", G_CALLBACK(need_data_callback), &UserData);
    g_signal_connect(appsrc, "enough-data", G_CALLBACK(enough_data_callback), &UserData);
    // Start playing
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
  
    // Create a GMainLoop to keep the application running
    pThread = new std::thread(&GStreamSender::SenderLoop, this);
    return true;
#endif

#if 0
    GstElement* pipeline, * appsrc, * videoconvert, * videoscale, * capsfilter, * udpsink;
    GstBus* bus;
    GstMessage* msg;
    GstCaps* caps;

    // Initialize GStreamer
    gst_init(NULL, NULL);

    // Create main event loop
    pLoop = g_main_loop_new(NULL, FALSE);
    pipeline = gst_parse_launch(
        "gst-launch-1.0 -v videotestsrc ! video/x-raw,format=YUY2,width=720,height=576,framerate=30/1 ! videoconvert ! rtpvrawpay ! udpsink host=127.0.0.1 port=1045",
        
        nullptr);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    pThread = new std::thread(&GStreamSender::SenderLoop, this);
    return true;
#endif

#if 1

    GstElement* pipeline, * appsrc, * videoconvert, * rtpvrawpay, * udpsink;
    GstBuffer* buffer;
    GstFlowReturn ret;
    GstMapInfo map;
    guint8* raw_data; // Your raw data buffer
    gsize raw_data_size = 0; // Size of your raw data

    // Initialize GStreamer
    gst_init(NULL,NULL);
    pLoop = g_main_loop_new(NULL, FALSE);
    // Create the elements
    appsrc = gst_element_factory_make("appsrc", "mysource");
    videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    rtpvrawpay = gst_element_factory_make("rtpvrawpay", "rtpvrawpay");
    udpsink = gst_element_factory_make("udpsink", "udpsink");

    // Create the empty pipeline
    pipeline = gst_pipeline_new("my-pipeline");

    if (!pipeline || !appsrc || !videoconvert || !rtpvrawpay || !udpsink) {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    // Build the pipeline
    gst_bin_add_many(GST_BIN(pipeline), appsrc, videoconvert, rtpvrawpay, udpsink, NULL);
    if (!gst_element_link_many(appsrc, videoconvert, rtpvrawpay, udpsink, NULL)) {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }
  

    // Configure the elements
    g_object_set(udpsink, "host", ip_addr.c_str(), "port", port, NULL);

    char capstr[1024];
    // Set appsrc properties
    sprintf_s(capstr, "video/x-raw,format=YUY2,width=%d,height=%d,framerate=%d/1", width, height, frame_rate);
    
    g_object_set(G_OBJECT(appsrc),
        //"caps", gst_caps_from_string("video/x-raw,format=YUY2,width=720,height=576,framerate=30/1"),
        "caps", gst_caps_from_string(capstr),
        "format", GST_FORMAT_TIME, NULL);

    // Start playing the pipeline

    TestImageBufferSize = width * height * 3;
    pTestImageBuffer = new uint8_t[TestImageBufferSize];
    memset(pTestImageBuffer, 0, TestImageBufferSize);
    pOutputBuffer = new uint8_t[width * height * 4];

    UserData.appsrc = appsrc;
    UserData.pSender = this;

    this->AppSrc = (appsrc); // Ensure AppSrc is a member of GStreamSender of type GstAppSrc*
    g_signal_connect(appsrc, "need-data", G_CALLBACK(need_data_callback), &UserData);
    g_signal_connect(appsrc, "enough-data", G_CALLBACK(enough_data_callback), &UserData);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    
    pThread = new std::thread(&GStreamSender::SenderLoop, this);

    return 0;
#endif
}

void GStreamSender::SenderLoop()
{
    while (true) {
        g_main_loop_run(pLoop);
    }
    
}

void GStreamSender::OnReceivedFrame(SSharedMemBufferHdr *p_hdr, const std::uint8_t* p_buffer, size_t width, size_t height)
{
    uint8_t* ret =  ConvertRGBAtoYCbCr(p_hdr, (uint8_t*)p_buffer,  width,  height);
    FeedData(ret, width * height * 4);
}


uint8_t* GStreamSender::ConvertRGBAtoYCbCr(SSharedMemBufferHdr *p_hdr,  std::uint8_t* p_buffer, size_t width, size_t height)
{
    uint8_t* yCbCrData = (uint8_t*)pOutputBuffer;
    int ind = 0;

    SSharedMemBufferHdr hdr_rcv = *p_hdr;

    hdr_rcv.BrightnessLevel = 0;
    hdr_rcv.ContrastLevel = 0;
    hdr_rcv.ImageInfo.IsIr = false;
    hdr_rcv.ImageInfo.IsWhiteHot = true;
    hdr_rcv.ImageInfo.EnableDefog = 0;
    hdr_rcv.ImageInfo.DefogLevel = 3;

    float brigtness_level = 0 + hdr_rcv.BrightnessLevel / 100.0 * 255;
    float constast_scale = 1 + 1.5 * hdr_rcv.ContrastLevel / 100;
    
    if (!hdr_rcv.ImageInfo.IsIr) {
        constast_scale += hdr_rcv.ImageInfo.IsICREnabled * 0.1;
    }
    
  
    INT16S high_res_pix[3];
    INT8U ir_val = 0;
    INT8U curr_pixel[3];
    INT16S fog_color[3] = { 150,150 , 100 };

    
    float defog_tf = 1- hdr_rcv.ImageInfo.DefogLevel / 3.0f;
#if true
    for (size_t i = 0; i < width * height * 4; i += 4) {

        if (hdr_rcv.ImageInfo.IsIr) {
            ir_val = p_buffer[i] * 0.299 + p_buffer[i + 1] * 0.587 + p_buffer[i + 2] * 0.114;
            if (!hdr_rcv.ImageInfo.IsWhiteHot) {
                ir_val = 255 - ir_val;
            }
            high_res_pix[0] = ir_val;
            high_res_pix[1] = ir_val;
            high_res_pix[2] = ir_val;
        }
        else {
            high_res_pix[0]   = p_buffer[i];
            high_res_pix[1]   = p_buffer[i + 1];
            high_res_pix[2]   = p_buffer[i + 2];
        }
  
        if (hdr_rcv.ImageInfo.EnableDefog) {
            high_res_pix[0] += CMath::Lerp(defog_tf, high_res_pix[0], fog_color[0]);
            high_res_pix[1] += CMath::Lerp(defog_tf, high_res_pix[1], fog_color[1]);
            high_res_pix[2] += CMath::Lerp(defog_tf, high_res_pix[2], fog_color[2]);
        }
        else {
            high_res_pix[0] += CMath::Lerp(1, high_res_pix[0], fog_color[0]);
            high_res_pix[1] += CMath::Lerp(1, high_res_pix[1], fog_color[1]);
            high_res_pix[2] += CMath::Lerp(1, high_res_pix[2], fog_color[2]);
        }


        high_res_pix[0] = constast_scale * high_res_pix[0] + brigtness_level;
        high_res_pix[1] = constast_scale * high_res_pix[1] + brigtness_level;
        high_res_pix[2] = constast_scale * high_res_pix[2] + brigtness_level;

        
        uint8_t R = CMath::ClampHighResPixel(high_res_pix[0], 0 ,255);
        uint8_t G = CMath::ClampHighResPixel(high_res_pix[1], 0, 255);
        uint8_t B = CMath::ClampHighResPixel(high_res_pix[2], 0, 255);
        // Alpha (p_buffer[i + 3]) is ignored

        p_buffer[i]     = R;
        p_buffer[i + 1] = G;
        p_buffer[i + 2] = B;

    }

#endif


    std::uint8_t* rgba = (std::uint8_t *)p_buffer;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; x += 2) {
            int idx = (y * width + x) * 4;

            // Process the first pixel
            int R1 = rgba[idx];
            int G1 = rgba[idx + 1];
            int B1 = rgba[idx + 2];
            int Y1 = (299 * R1 + 587 * G1 + 114 * B1) / 1000;
            int U1 = (-169 * R1 - 331 * G1 + 500 * B1) / 1000 + 128;


            // Process the second pixel
            idx += 4; // Move to the next pixel in the RGBA buffer
            int R2 = rgba[idx];
            int G2 = rgba[idx + 1];
            int B2 = rgba[idx + 2];
            int Y2 = (299 * R2 + 587 * G2 + 114 * B2) / 1000;
            int V1 = (500 * R1 - 419 * G1 - 81 * B1) / 1000 + 128;

            // Averaging U and V values of the two pixels
            int U = (U1 + (-169 * R2 - 331 * G2 + 500 * B2) / 1000 + 128) / 2;
            int V = (V1 + (500 * R2 - 419 * G2 - 81 * B2) / 1000 + 128) / 2;

            // Clamping the Y, U, and V values to 0-255
            Y1 = CMath::Clamp(Y1, 0, 255);
            U = CMath::Clamp(U, 0, 255);
            Y2 = CMath::Clamp(Y2, 0, 255);
            V = CMath::Clamp(V, 0, 255);

            int j = (y * width + x) * 2;

         
            yCbCrData[j] = static_cast<uint8_t>(Y1);
            yCbCrData[j + 1] = static_cast<uint8_t>(V );
            yCbCrData[j + 2] = static_cast<uint8_t>(Y2);
            yCbCrData[j + 3] = static_cast<uint8_t>(U);
        
        }
    }
   
    return pOutputBuffer;
}

void GStreamSender::FeedData(uint8_t* data, int size) {

    if (pLastBuff == NULL) {
        pLastBuff = new uint8_t[size];
    }

    memcpy(pLastBuff, data, size);
    LastSize = size;
     
    
 
    if (!this->AppSrc) {
        return; // AppSrc is not initialized
    }

    GstClockTime buffer_duration = gst_util_uint64_scale_int(1, GST_SECOND, 30); // 1/30th of a second
   
    GstBuffer* buffer = gst_buffer_new_allocate(NULL, size, NULL);
    auto filled = gst_buffer_fill(buffer, 0, data, size);
    // Set buffer PTS. DTS is not set here as it's often not necessary for simple cases.
    GST_BUFFER_PTS(buffer) = TimeStamp;
    GST_BUFFER_DURATION(buffer) = buffer_duration;

    TimeStamp += buffer_duration;
    GstFlowReturn ret;
   

    g_signal_emit_by_name(this->AppSrc, "push-buffer", buffer, &ret);

    if (ret != GST_FLOW_OK) {
        // Handle error
    }

    gst_buffer_unref(buffer);

    //g_timer_start(this->AppSrc->timer);
}