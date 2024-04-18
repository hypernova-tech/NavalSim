#include "CFlsIF.h"
#include <iostream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "zeromq/include/zmq.h"
#include "zeromq/include/zmq.hpp"
#include "proto/nav_api.pb.h"

const std::string CFlsIF::TargetDataPort = "61502";
const std::string CFlsIF::FovPort = "60502";
const std::string CFlsIF::SquelchPort = "60504";
const std::string CFlsIF::AutoSquelchPort = "60505";
const std::string CFlsIF::BottomDetectionPort = "60503";
const std::string CFlsIF::GetProcessingSettingPort = "60501";
const std::string CFlsIF::PublisProcessingSettinghPort = "61503";

extern bool ProtoMessageToZeromqMessage__(
    const ::google::protobuf::MessageLite& proto_message,
    zmq::message_t* zeromq_message);

CFlsIF::CFlsIF()
{
    pTargetDataPublisherSoc = nullptr;
}

void  CFlsIF::SetHeader(SSharedMemBufferHdr* p_val)
{
    pHdr = p_val;
}
SSharedMemBufferHdr* CFlsIF::GetHeader()
{
    return pHdr;
}

zmq::context_t context(1);
void CFlsIF::UpdateFlsTargetData(SSharedMemBufferHdr hdr, SFLSDataEntry* p_entries, INT32U count)
{

 

    if (count <= 0) {
        return;
    }

   
    if (pTargetDataPublisherSoc == nullptr) {
        
        pTargetDataPublisherSoc = new zmq::socket_t(context, ZMQ_PUB);
        pTargetDataPublisherSoc->bind("tcp://*:" + CFlsIF::TargetDataPort); // Adjust the port and protocol as needed
        
    }


    
    proto::nav_api::TargetData targetData;

    // Fill in the shared memory buffer header data
    // Example: Position and orientation of the sonar device
    targetData.mutable_position()->set_lat(hdr.PosLLH[0]);
    targetData.mutable_position()->set_lon(hdr.PosLLH[1]);
    // Assuming there's a field for altitude or height in your protobuf definition
    
    double heading = atan2(hdr.LookDir[0], hdr.LookDir[1]) * RADTODEG;
    targetData.mutable_heading()->set_heading(heading);

    auto* group = targetData.add_groups();
    auto own_ship = hdr.PosXYZ;
    // Iterate over the entries to fill in the target or ground data
    for (unsigned int i = 0; i < count; ++i) {
        
        int horr_index = 0;
        int ver_index = 0;
        int range_index = 0;
        float strength = 1;

        
        if (p_entries[i].Info.IsGround) {
            // Assuming you have a method to add points to a "bottom" representation in your protobuf
            auto* bin = targetData.add_bottom();
            bin->set_cross_range(p_entries[i].X - own_ship[0]); // Adjust field names as needed
            bin->set_down_range(p_entries[i].Y - own_ship[1]);
            bin->set_depth(-(p_entries[i].Z - own_ship[2]));
            bin->set_hor_index(horr_index);
            bin->set_ver_index(ver_index);
            bin->set_range_index(range_index);
            bin->set_strength(strength);
        }
        else {
            // For non-ground targets, assumed to be in-water targets
            
            auto* bin = group->add_bins();
            bin->set_cross_range(p_entries[i].X- own_ship[0]);
            bin->set_down_range(p_entries[i].Y - own_ship[1]);
            bin->set_depth(-(p_entries[i].Z - own_ship[2]));
            bin->set_hor_index(horr_index);
            bin->set_ver_index(ver_index);
            bin->set_range_index(range_index);
            bin->set_strength(strength);
        }
    }
    

#if 0

    // Serialize the TargetData message
    std::string serializedData;
    if (!targetData.SerializeToString(&serializedData)) {
        std::cerr << "Failed to serialize TargetData." << std::endl;
        return;
    }
    // Publish the serialized TargetData
    zmq::message_t message(serializedData.begin(), serializedData.end());

#endif

 
    zmq::message_t message;
    ProtoMessageToZeromqMessage__(targetData, &message);
    pTargetDataPublisherSoc->send(message, zmq::send_flags::none);


}

void CFlsIF::SetRangeMeter(double val)
{
    pHdr->FromHostToSim.RangeMeter = val;
}

double CFlsIF::GetRangeMeter()
{
    return pHdr->FromSimToHost.RangeMeter;
}

void CFlsIF::SetFLSOn(bool val)
{
    pHdr->FromHostToSim.IsFlsOn = val;
}

bool CFlsIF::GetFLSOn()
{
    pHdr->FromSimToHost.IsFlsOn;
}

void CFlsIF::SetAutoSquelch(bool val)
{
    pHdr->FromHostToSim.IsAutoSquelchEnabled = val;
}

bool CFlsIF::GetAutoSquelch()
{
    return pHdr->FromSimToHost.IsAutoSquelchEnabled;
}

void CFlsIF::SetBottomDetection(bool val)
{
    pHdr->FromHostToSim.BottomDetectionEnabled = val;
}

bool CFlsIF::GetBottomDetection()
{
    return pHdr->FromSimToHost.BottomDetectionEnabled;
}

void CFlsIF::SetInWaterSquelch(double val)
{
    pHdr->FromHostToSim.SquelchSensitivity = val;
}

double CFlsIF::GetInWaterSquelch()
{
    return pHdr->FromHostToSim.SquelchSensitivity;
}
