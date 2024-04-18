// SDKMessageExample : minimal example implementation of request/reply and
// publish/subscribe style method passing.
/*
Copyright (c) 2023 FarSounder
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iostream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "zeromq/include/zmq.h"
#include "zeromq/include/zmq.hpp"

#include "proto/nav_api.pb.h"
#include "CSharedMemManager.h"
#include "CFLSHostListener.h"

#define STANDALONE_EN 1

const std::string TcpPortNo = "4152";

const int kLargeMessageBytesLimit = 128 << 20;  // 128MB
const int kLargeMessageBytesWarningThreshold = 110 << 20;  // 110MB


// Used by ProtoMessageToZeromqMessage for zeromq zero copy. See
// http://www.zeromq.org/blog:zero-copy
void zeromq_free(void *data, void *hint) {
  free(data);
}

// Unless you're sure you're reading smaller sized messages (ie- you're not
// writing a general base level utility) you should use this function to parse
// protobuf messages so that if you end up parsing something large you'll be
// able to.
//
// See also:
//  protos/protobuf-bin/include/google/protobuf/io/coded_stream.h's
//  SetTotalBytesLimit function.
template <class ProtoMessage>
inline bool ParseLargeMessage(const void *data, int data_len, ProtoMessage *proto_msg) {
  google::protobuf::io::ArrayInputStream array_stream(data, data_len);
  google::protobuf::io::CodedInputStream coded_stream(&array_stream);
  coded_stream.SetTotalBytesLimit(kLargeMessageBytesLimit);
  bool success = proto_msg->ParseFromCodedStream(&coded_stream);
  return success;
}

// Uses zero copy to get data into proto message after serialization.
bool ProtoMessageToZeromqMessage(
  const ::google::protobuf::MessageLite &proto_message,
  zmq::message_t *zeromq_message) {
  int string_size = proto_message.ByteSizeLong();
  // We malloc a char* instead of using a std::string so we can pass ownership
  // to the zeromq_message. It will take care of calling free().
  void *string = malloc(string_size);
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


bool ZeromqMessageToProtoMessage(
  const zmq::message_t &zeromq_message,
  ::google::protobuf::MessageLite *proto_message) {
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

template <class ProtoMessageRequest, class ProtoMessageResponse>
void send_generic_request(
    ProtoMessageRequest req, std::string port,
    ProtoMessageResponse &res){
  zmq::context_t context(1);
  zmq::socket_t zmq_socket(context, ZMQ_REQ);
  std::string address = "tcp://localhost:";
  zmq_socket.connect(address + port);

  // Proto request to zmq message
  zmq::message_t update;
  ProtoMessageToZeromqMessage(req, &update);
  printf("Send request\n");
  zmq_socket.send(update, zmq::send_flags::none);
  printf("Check for response\n");
  zmq_socket.recv(update, zmq::recv_flags::none);
  // zmq response back to a proto message
  ZeromqMessageToProtoMessage(update, &res);
  printf("result=%d\n", res.result().code());
  printf("res_detail=%s\n", res.result().result_detail().c_str());

}

///////////////////////////////////////////////////////////////////////////////
// Examples

void request_reply_example()
{
  /////////////////////////////////////////////
  // ZeroMQ pattern: Request-reply
  /////////////////////////////////////////////
  zmq::context_t context(1);
  zmq::socket_t req_processorSettings(context, ZMQ_REQ);
  std::string address = "tcp://localhost:";
  std::string port = "60501";
  req_processorSettings.connect(address + port);

  proto::nav_api::GetProcessorSettingsRequest  gpsReq;
  proto::nav_api::GetProcessorSettingsResponse gpsResp;
  // Proto request to zmq message
  zmq::message_t update;
  ProtoMessageToZeromqMessage(gpsReq, &update);
  printf("Send request\n");
  req_processorSettings.send(update, zmq::send_flags::none);
  printf("Check for response\n");
  req_processorSettings.recv(update, zmq::recv_flags::none);
  // zmq response back to a proto message
  ZeromqMessageToProtoMessage(update, &gpsResp);
  printf("result=%d\n", gpsResp.result().code());
  printf("system_type=%d\n",gpsResp.settings().system_type());
}

void toggle_bottom_detection(bool bottom_detect_on)
{
  std::string port = "60503";

  proto::nav_api::SetBottomDetectionRequest bottom_detect_request;
  proto::nav_api::SetBottomDetectionResponse bottom_detect_response;
  bottom_detect_request.set_enable_bottom_detection(bottom_detect_on);
  printf("bottom detect value: %d\n", bottom_detect_on);
  send_generic_request(bottom_detect_request, port, bottom_detect_response);
}

void toggle_auto_squelch(bool auto_squelch_on) {
  std::string port = "60505";

  proto::nav_api::SetSquelchlessInWaterDetectorRequest request;
  proto::nav_api::SetSquelchlessInWaterDetectorResponse response;
  request.set_enable_squelchless_detection(auto_squelch_on);
  printf("auto squelch value: %d\n", auto_squelch_on);
  send_generic_request(request, port, response);
}

void set_squelch(float squelch) {
  std::string port = "60504";

  proto::nav_api::SetInWaterSquelchRequest request;
  proto::nav_api::SetInWaterSquelchResponse response;
  request.set_new_squelch_val(squelch);
  printf("squelch value: %f\n", squelch);
  send_generic_request(request, port, response);
}

void set_fov(proto::nav_api::FieldOfView new_fov) {
  std::string port = "60502";
  proto::nav_api::SetFieldOfViewRequest request;
  proto::nav_api::SetFieldOfViewResponse response;
  request.set_fov(new_fov);
  printf("fov enum number: %d\n", new_fov);
  send_generic_request(request, port, response);
}

void publish_subscribe_example()
{
  /////////////////////////////////////////////
  // ZeroMQ pattern: Publish-subscribe
  /////////////////////////////////////////////
  zmq::context_t context(1);
  zmq::socket_t subscr_processorSettings(context, ZMQ_SUB);
  std::string address = "tcp://localhost:";
  std::string port = "61503";

  subscr_processorSettings.connect(address + port);
  subscr_processorSettings.set(zmq::sockopt::subscribe, ""); 
  bool loop = true;
  while (loop)
  {
    zmq::message_t update;
    if (subscr_processorSettings.recv(update, zmq::recv_flags::dontwait))
    {
      proto::nav_api::ProcessorSettings processorSettings;
      if (processorSettings.ParseFromArray(update.data(), update.size()))
        printf("system_type=%d\n", processorSettings.system_type());
    }
  }
}
/*
void UpdateFov()
{
    std::string port = "60502";
    zmq::context_t context(1);
    zmq::socket_t zmq_socket(context, ZMQ_REQ);
    std::string address = "tcp://localhost:";
    zmq_socket.connect(address + port);
    proto::nav_api::SetFieldOfViewRequest request;
    proto::nav_api::SetFieldOfViewResponse response;
    // Proto request to zmq message
    zmq::message_t update;
    update.rebuild();
    zmq_socket.recv(update, zmq::recv_flags::none);
    // zmq response back to a proto message
    ZeromqMessageToProtoMessage(update, &request);

}
*/
void ZeroMQProtoServer() {
    std::string port = "60502";
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:" + port);

    while (true) {
        zmq::message_t request_msg;

        // Wait for next request from client
        socket.recv(request_msg, zmq::recv_flags::none);

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
       
        socket.send(res_msg, zmq::send_flags::none);
    }
}

proto::nav_api::TargetData CreateTargetData() {
    proto::nav_api::TargetData target_data;

    // Set the timestamp (example)
    //target_data.mutable_time()->set_seconds(std::time(nullptr));
    //target_data.mutable_time()->set_nanos(0);

    // Set the sonar serial number
    target_data.set_serial("YourSonarSerialNumber");

    // Assuming you have functions to get heading and position
    auto heading = 0;
    auto lat = 0;
    auto lon = 0;
    target_data.mutable_heading()->set_heading(heading); //->set_degrees(heading);
    target_data.mutable_position()->set_lat(lat);
    target_data.mutable_position()->set_lon(lon);

    // Populate the bottom and groups with Bin data
    // This is highly dependent on your data processing logic
    // Here's a simple example for adding one Bin to the bottom
    auto* bottom_bin = target_data.add_bottom();
    bottom_bin->set_hor_index(0);  // Example values
    bottom_bin->set_ver_index(0);
    bottom_bin->set_range_index(1);
    bottom_bin->set_cross_range(10.0);
    bottom_bin->set_down_range(20.0);
    bottom_bin->set_depth(5.0);
    bottom_bin->set_strength(0.8);

    // Similar logic would apply for adding Bins to groups

    // Set grid description, max_depth, and max_range_index as needed
    // target_data.mutable_grid_description()->...
    // target_data.set_max_depth(50.0);
    // target_data.set_max_range_index(100);

    return target_data;
}

#if STANDALONE_EN > 0
CSharedMemManager* pSharedMemManager;
CFLSHostListener* pHostListener;
CFlsIF* pFLSIf;


int main()
{
    std::string smAddress = "FLSSM1";
    int memSize = sizeof(SFLSSharedMemBufferHdr) + 1920 * 1080 * sizeof(SFLSDataEntry);
    

   
    pSharedMemManager = new CSharedMemManager();
    pHostListener = new CFLSHostListener();
    pFLSIf = new CFlsIF();

    SAppArgs args;
    args.ShareMemName = smAddress;
    args.Size = memSize;
    args.pFlsIf = pFLSIf;
    pSharedMemManager->Init(args);
    pHostListener->Init(pFLSIf);
    while (true);
    
    
    ZeroMQProtoServer();

   
    
    std::cout << "Test Communication with SonaSoft API\n";

    // Requests latest system type and parses reply - runs once
    ////request_reply_example();

    /*
    // Subscribes to sytem type message - this loops forever (uncomment to run)
    publish_subscribe_example();
    */

    //
    // Test toggling processor controls
    //
    bool bottom_detect = false;
    bool auto_squelch_on = false;
    float squelch_value = 150.0f;
    while (true) {

        // Change field of view
        printf("About to change field of view / processor mode to: %d\n",
            proto::nav_api::FieldOfView::k120d100m);
        system("pause");
        set_fov(proto::nav_api::FieldOfView::k120d100m);

      // Bottom detection
      printf("About to send request to change bottom to: %d\n", bottom_detect);
      system("pause");
      toggle_bottom_detection(bottom_detect);
      printf("\n");

      // Auto squelch
      printf("About to send request to change autosquelch to: %d\n",
        auto_squelch_on);
      system("pause");
      toggle_auto_squelch(auto_squelch_on);
      printf("\n");

      if (!auto_squelch_on) {
        // Change Squelch value (only makes sense for manual squelch)
        printf("About to send request to manual squelch to: %f\n",
          squelch_value);
        system("pause");
        set_squelch(squelch_value);
        squelch_value = (squelch_value == 150.0f) ? 170.0f : 150.0f;
        printf("\n");
      }



      printf("About to change field of view / processor mode to: %d\n",
        proto::nav_api::FieldOfView::k120d200m);
      system("pause");
      set_fov(proto::nav_api::FieldOfView::k120d200m);


      printf("About to change field of view / processor mode to: %d\n",
        proto::nav_api::FieldOfView::k90d500m);
      system("pause");
      set_fov(proto::nav_api::FieldOfView::k90d500m);
 
      // Toggle bools
      auto_squelch_on = !auto_squelch_on;
      bottom_detect = !bottom_detect;
    }
}


#else
// Declare the function with the export/import macro
extern "C" {  // Use extern "C" if you want to ensure C linkage (name mangling avoidance)
    __declspec(dllexport) int CreateInstance(char *sm_sname, int size);
}

struct SInstanceParams
{
    SAppArgs Args;
    CSharedMemManager* pSharedMemManager;
    CFLSHostListener* pHostListener;
};

list< SInstanceParams*> InstancesParams;



// Function definition
int CreateInstance(char *sm_sname, int size) {
    // Implement your initialization logic here 
    cout << "initing sonar model" << endl;
    SInstanceParams* p_args = new SInstanceParams();

    p_args->pSharedMemManager = new CSharedMemManager();
    p_args->pHostListener = new CFLSHostListener();

    SAppArgs args;
    args.ShareMemName = string(sm_sname);
    args.Size = size;
    p_args->Args = args;
    p_args->pSharedMemManager->Init(args);
    p_args->pHostListener->Init();
    InstancesParams.push_back(p_args);
    return InstancesParams.size();
    //while (true);
}

#endif