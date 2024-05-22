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
    //std::cout << "appsrc has enough data, pausing or slowing data feed." << std::endl;
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



inline void RGBToYuv(int R, int G, int B, int& Y, int& U, int& V)
{



    Y = (0.257 * R) + (0.504 * G) + (0.098 * B) + 16;
    V = (0.439 * R) - (0.368 * G) - (0.071 * B) + 128;
    U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128;



   // Y = static_cast<int>(std::round((0.299 * R + 0.587 * G + 0.114 * B)));
  //  U = static_cast<int>(std::round( - 0.168736 * R - 0.331264 * G + 0.5 * B + 128));
   // V = static_cast<int>(std::round(0.5 * R -0.418688 * G - 0.081312 * B + 128));


  
}

void rgbToYCbCr422(uint8_t* rgba, uint8_t* yCbCrData, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; x += 2) {
            int idx = (y * width + x) * 4;

            // Process the first pixel
            int R1 = rgba[idx];
            int G1 = rgba[idx + 1];
            int B1 = rgba[idx + 2];

            int Y1;
            int U1;
            int V1;

            RGBToYuv(R1, G1, B1, Y1, U1, V1);

            // Move to the next pixel in the RGBA buffer
            idx += 4;
            int R2 = rgba[idx];
            int G2 = rgba[idx + 1];
            int B2 = rgba[idx + 2];

            int Y2 ;
            int U2 ;
            int V2 ;

 
            RGBToYuv(R2, G2, B2, Y2, U2, V2);

            // Averaging U and V values of the two pixels
            int U = (U1 + U2) / 2;
            int V = (V1 + V2) / 2;

            // Clamping the Y, U, and V values to 0-255
            Y1 = CMath::Clamp(Y1, 0, 255);
            U = CMath::Clamp(U, 0, 255);
            Y2 = CMath::Clamp(Y2, 0, 255);
            V = CMath::Clamp(V, 0, 255);

            int j = (y * width + x) * 2;

            yCbCrData[j] = static_cast<uint8_t>(Y1);
            yCbCrData[j + 1] = static_cast<uint8_t>(V);
            yCbCrData[j + 2] = static_cast<uint8_t>(Y2);
            yCbCrData[j + 3] = static_cast<uint8_t>(U);
        }
    }
}

uint8_t* GStreamSender::ConvertRGBAtoYCbCr(SSharedMemBufferHdr *p_hdr,  std::uint8_t* p_buffer, size_t width, size_t height)
{
    uint8_t* yCbCrData = (uint8_t*)pOutputBuffer;
    int ind = 0;

    SSharedMemBufferHdr hdr_rcv = *p_hdr;

#if 0
    hdr_rcv.BrightnessLevel = 0;
    hdr_rcv.ContrastLevel = 0;
    hdr_rcv.ImageInfo.IsIr = false;
    hdr_rcv.ImageInfo.IsWhiteHot = true;
    hdr_rcv.ImageInfo.EnableDefog = 0;
    hdr_rcv.ImageInfo.DefogLevel = 3;
#endif
    hdr_rcv.BrightnessLevel = 50;
    hdr_rcv.ContrastLevel = 50;

    float brigtness_level = 0 + hdr_rcv.BrightnessLevel/5.0 / 100.0 * 255;
    if (hdr_rcv.ImageInfo.IsIr) {
        if (!hdr_rcv.ImageInfo.IsWhiteHot) {
            brigtness_level = CMath::Lerp(hdr_rcv.BrightnessLevel / 100.0, 50, -50);
        }
        else {
            brigtness_level = CMath::Lerp(hdr_rcv.BrightnessLevel / 100.0, -50, 50);
        }
    }
    else {
        brigtness_level = CMath::Lerp(hdr_rcv.BrightnessLevel / 100.0, -50, 50);
    }
   
    float constast_scale = 1 + hdr_rcv.ContrastLevel / 150;
    
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
       
            high_res_pix[0] = ir_val;
            high_res_pix[1] = ir_val;
            high_res_pix[2] = ir_val;
        }
        else {
            high_res_pix[0]   = p_buffer[i];
            high_res_pix[1]   = p_buffer[i + 1];
            high_res_pix[2]   = p_buffer[i + 2];
        }
  
        if (!hdr_rcv.ImageInfo.IsIr) {
            if (hdr_rcv.ImageInfo.EnableDefog) {
                //high_res_pix[0] += CMath::Lerp(defog_tf, high_res_pix[0], fog_color[0]);
                //high_res_pix[1] += CMath::Lerp(defog_tf, high_res_pix[1], fog_color[1]);
                //high_res_pix[2] += CMath::Lerp(defog_tf, high_res_pix[2], fog_color[2]);


                high_res_pix[0] -= CMath::Lerp(defog_tf,  0.3*fog_color[0],0);
                high_res_pix[1] -= CMath::Lerp(defog_tf, 0.3*fog_color[1],0);
                high_res_pix[2] -= CMath::Lerp(defog_tf,  0.3*fog_color[2],0);

    
            }
            else {

               // high_res_pix[0] += CMath::Lerp(1, high_res_pix[0], fog_color[0]);
               // high_res_pix[1] += CMath::Lerp(1, high_res_pix[1], fog_color[1]);
               // high_res_pix[2] += CMath::Lerp(1, high_res_pix[2], fog_color[2]);
            }
        }


        high_res_pix[0] = constast_scale * high_res_pix[0] + brigtness_level;
        high_res_pix[1] = constast_scale * high_res_pix[1] + brigtness_level;
        high_res_pix[2] = constast_scale * high_res_pix[2] + brigtness_level;

        
        uint8_t R = CMath::ClampHighResPixel(high_res_pix[0], 0 ,255);
        uint8_t G = CMath::ClampHighResPixel(high_res_pix[1], 0, 255);
        uint8_t B = CMath::ClampHighResPixel(high_res_pix[2], 0, 255);
        // Alpha (p_buffer[i + 3]) is ignored

        if (hdr_rcv.ImageInfo.IsIr) {
            if (!hdr_rcv.ImageInfo.IsWhiteHot) {
                R = 255 - R;
                G = 255 - G;
                B = 255 - B;
            }
        }

        p_buffer[i]     = R;
        p_buffer[i + 1] = G;
        p_buffer[i + 2] = B;

    }

#endif


    std::uint8_t* rgba = (std::uint8_t *)p_buffer;

#if 0
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; x += 2) {
            int idx = (y * width + x) * 4;

            /*matlab
                Y = 0.257R´ + 0.504G´ + 0.098B´ + 16 -->Y

                Cb = -0.148R´ - 0.291G´ + 0.439B´ + 128 --> U

                Cr = 0.439R´ - 0.368G´ - 0.071B´ + 128  -->V
            */
            // Process the first pixel
            int R1 = rgba[idx];
            int G1 = rgba[idx + 1];
            int B1 = rgba[idx + 2];
            int Y1 = (299 * R1 + 587 * G1 + 114 * B1) / 1000.0; //Y
            int U1 = (-169 * R1 - 331 * G1 + 500 * B1) / 1000.0 + 128; //Cb


            // Process the second pixel
            idx += 4; // Move to the next pixel in the RGBA buffer
            int R2 = rgba[idx];
            int G2 = rgba[idx + 1];
            int B2 = rgba[idx + 2];
            int Y2 = (299 * R2 + 587 * G2 + 114 * B2) / 1000.0;
            int V1 = (500 * R1 - 419 * G1 - 81 * B1) / 1000.0 + 128; //cr

            // Averaging U and V values of the two pixels
            int U = (U1 + (-169 * R2 - 331 * G2 + 500 * B2) / 1000.0 + 128) / 2;
            int V = (V1 + (500 * R2 - 419 * G2 - 81 * B2) / 1000.0 + 128) / 2;

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
#endif

    
    rgbToYCbCr422(rgba, yCbCrData, width, height);
   
    return pOutputBuffer;
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

    //g_timer_start(this->AppSrc->timer);
}