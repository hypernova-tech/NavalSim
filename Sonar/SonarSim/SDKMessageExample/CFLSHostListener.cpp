#include "CFLSHostListener.h"
#include <iostream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "zeromq/include/zmq.h"
#include "zeromq/include/zmq.hpp"
#include "proto/nav_api.pb.h"
#include "CSharedMemManager.h"

const int kLargeMessageBytesLimit = 128 << 20;  // 128MB
const int kLargeMessageBytesWarningThreshold = 110 << 20;  // 110MB
void CFLSHostListener::Init()
{
    pThread = new thread(&CFLSHostListener::ThreadFunc, this);
    
}

// Used by ProtoMessageToZeromqMessage for zeromq zero copy. See
// http://www.zeromq.org/blog:zero-copy
static void zeromq_free(void* data, void* hint) {
    free(data);
}

template <class ProtoMessage>
static inline bool ParseLargeMessage(const void* data, int data_len, ProtoMessage* proto_msg) {
    google::protobuf::io::ArrayInputStream array_stream(data, data_len);
    google::protobuf::io::CodedInputStream coded_stream(&array_stream);
    coded_stream.SetTotalBytesLimit(kLargeMessageBytesLimit);
    bool success = proto_msg->ParseFromCodedStream(&coded_stream);
    return success;
}

static bool ZeromqMessageToProtoMessage(const zmq::message_t& zeromq_message,     ::google::protobuf::MessageLite* proto_message)
{
    bool success = ParseLargeMessage(
        zeromq_message.data(), static_cast<int>(zeromq_message.size()),
        proto_message);
    if (!success) {
        // The message is garbage or too big. The garbage can happen if zeromq picks
        // up some bad data off a port - luckily Protobuf knows when the data is
        // terrible. If the message is too big we'll need to update the
        // ParseLargeMessage's size cutoffs.
        // ...
        // handle / log error
        // ...
    }
    return success;
}
static bool ProtoMessageToZeromqMessage(
    const ::google::protobuf::MessageLite& proto_message,
    zmq::message_t* zeromq_message) {
    int string_size = proto_message.ByteSizeLong();
    // We malloc a char* instead of using a std::string so we can pass ownership
    // to the zeromq_message. It will take care of calling free().
    void* string = malloc(string_size);
    if (string == NULL) {
        // ...
        // handle / log error
        // ...
        return false;
    }
    if (!proto_message.SerializeToArray(string, string_size)) {
        // ...
        // handle / log error
        // ...
        free(string);
        return false;
    }
    zeromq_message->rebuild(string, string_size, zeromq_free);
    return true;
}

void CFLSHostListener::ZeroMQProtoServer() {
    std::string port;
    vector< string> ports = { "60502", "60503" };

    vector< zmq::context_t> contexts;

    vector<zmq::socket_t*> sockets;// socket(context, ZMQ_REP);
   
    auto context = zmq::context_t( 1);
    for (int i = 0; i < ports.size(); i++) {
        port = ports.at(i);
        
        zmq::socket_t* p_soc = new zmq::socket_t(context, ZMQ_REP);
        p_soc->bind("tcp://*:" + port);
        sockets.push_back(p_soc);
        
    }

    while (true) {

        for (int i = 0; i<sockets.size(); i++) {
            auto port = ports.at(i);
            zmq::message_t request_msg;
            zmq::socket_t* p_socket = sockets.at(i);
            
            // Wait for next request from client
            auto res = p_socket->recv(request_msg, zmq::recv_flags::dontwait);
          
            if (res.has_value()) {

                if (CFlsIF::FovPort == port) {
                    // Deserialize request from message
                    proto::nav_api::SetFieldOfViewRequest request;
                    auto ret = ZeromqMessageToProtoMessage(request_msg, &request);
                    if (!ret) {
                        std::cerr << "Failed to parse SetFieldOfViewRequest." << std::endl;
                        continue; // Skip to next iteration on parse failure
                    }

                    // Process request (this is where you would add your logic)
                    proto::nav_api::SetFieldOfViewResponse response;

                    zmq::message_t res_msg;
                    // Populate response based on the request
                    ProtoMessageToZeromqMessage(response, &res_msg);
                    p_socket->send(res_msg, zmq::send_flags::none);
                }
                else if (CFlsIF::BottomDetectionPort == port) {
                    proto::nav_api::SetBottomDetectionRequest request;
                    auto ret = ZeromqMessageToProtoMessage(request_msg, &request);
                    if (!ret) {
                        std::cerr << "Failed to parse SetFieldOfViewRequest." << std::endl;
                        continue; // Skip to next iteration on parse failure
                    }

                    // Process request (this is where you would add your logic)
                    proto::nav_api::SetBottomDetectionResponse response;

                    zmq::message_t res_msg;
                    // Populate response based on the request
                    ProtoMessageToZeromqMessage(response, &res_msg);
                    p_socket->send(res_msg, zmq::send_flags::none);
                }
               

               
            }
     
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        
    }
}

void CFLSHostListener::ThreadFunc()
{
    ZeroMQProtoServer();
}