#include "GStreamSender.h"
#include <iostream>



bool GStreamSender::Init(string name, string ip_addr, int port, int width, int height, int frame_rate)
{
    GstElement* pipeline, * appsrc, * conv, * payloader, * sink;
  

    pOut = new uint8_t[width * height * 3];

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
        "format", G_TYPE_STRING, "YCbCr-4:2:2",
        "width", G_TYPE_INT, width,
        "height", G_TYPE_INT, height,
        "framerate", GST_TYPE_FRACTION, frame_rate, 1,
        NULL);
    g_object_set(G_OBJECT(appsrc), "caps", caps, "format", GST_FORMAT_TIME, NULL);
   

  

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

    // Start playing
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // You would typically not run the main loop here in a real application
    gst_caps_unref(caps);

    // Instead of running the loop here, you might want to run it in a separate thread or use async mechanisms
    pThread = new std::thread(&GStreamSender::SenderLoop, this);
    return true;

}

void GStreamSender::SenderLoop()
{
    while (true) {
        g_main_loop_run(pLoop);
    }
    
}

void GStreamSender::OnReceivedFrame(const std::uint8_t* p_buffer, size_t width, size_t height)
{
    uint8_t* ret =  ConvertRGBAtoYCbCr(p_buffer,  width,  height);
    FeedData(ret, width * height * 3);
}


uint8_t* GStreamSender::ConvertRGBAtoYCbCr(const std::uint8_t* p_buffer, size_t width, size_t height)
{
    uint8_t* yCbCrData = (uint8_t*)p_buffer;
    int ind = 0;
  
    for (size_t i = 0; i < width * height * 4; i += 4) {
        uint8_t R = p_buffer[i];
        uint8_t G = p_buffer[i + 1];
        uint8_t B = p_buffer[i + 2];
        // Alpha (p_buffer[i + 3]) is ignored

        // Apply conversion formulas
        uint8_t Y = (uint8_t)(0.299 * R + 0.587 * G + 0.114 * B);
        uint8_t Cb = (uint8_t)(-0.168736 * R - 0.331264 * G + 0.5 * B + 128);
        uint8_t Cr = (uint8_t)(0.5 * R - 0.418688 * G - 0.081312 * B + 128);

        // Append Y, Cb, and Cr to the output buffer
        yCbCrData[ind++] = (Y);
        yCbCrData[ind++] = (Cb);
        yCbCrData[ind++] = (Cr);
    }

    return yCbCrData;
}

void GStreamSender::FeedData(uint8_t* data, int size) {
    if (!this->AppSrc) {
        return; // AppSrc is not initialized
    }

    GstBuffer* buffer = gst_buffer_new_allocate(NULL, size, NULL);
    gst_buffer_fill(buffer, 0, data, size);

    GstFlowReturn ret;
    g_signal_emit_by_name(this->AppSrc, "push-buffer", buffer, &ret);

    if (ret != GST_FLOW_OK) {
        // Handle error
    }

    gst_buffer_unref(buffer);

   // g_timer_start(this->AppSrc->timer);
}