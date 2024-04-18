#include "CFlsIF.h"
#include <iostream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "zeromq/include/zmq.h"
#include "zeromq/include/zmq.hpp"
#include "proto/nav_api.pb.h"
#include "CMath.h"

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
    pProcessorSettingsSoc = nullptr;
}

void  CFlsIF::SetHeader(SFLSSharedMemBufferHdr* p_val)
{
    pHdr = p_val;
}
SFLSSharedMemBufferHdr* CFlsIF::GetHeader()
{
    return pHdr;
}

zmq::context_t context(1);
void CFlsIF::UpdateFlsTargetData(SFLSSharedMemBufferHdr hdr, SFLSDataEntry* p_entries, INT32U count)
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
        float strength = 200;

        
        if (p_entries[i].Info.IsGround) {
            // Assuming you have a method to add points to a "bottom" representation in your protobuf
            auto* bin = targetData.add_bottom();
            bin->set_down_range(p_entries[i].X - own_ship[0]);
            bin->set_cross_range(-(p_entries[i].Y - own_ship[1])); // Adjust field names as needed
            bin->set_depth((p_entries[i].Z - own_ship[2]));
            bin->set_hor_index(horr_index);
            bin->set_ver_index(ver_index);
            bin->set_range_index(range_index);
            bin->set_strength(strength);
        }
        else {
            // For non-ground targets, assumed to be in-water targets
            
            auto* bin = group->add_bins();
            bin->set_down_range(p_entries[i].X - own_ship[0]);
            bin->set_cross_range(-(p_entries[i].Y - own_ship[1])); // Adjust field names as needed
            bin->set_depth((p_entries[i].Z - own_ship[2]));
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

void CFlsIF::PublishProcessorSettingData()
{
    if (IsHostDataDirty) {
        IsHostDataDirty = false;
    }
    else {
        return;
    }
    if (pProcessorSettingsSoc == nullptr) {

        pProcessorSettingsSoc = new zmq::socket_t(context, ZMQ_PUB);
        pProcessorSettingsSoc->bind("tcp://*:" + CFlsIF::PublisProcessingSettinghPort); // Adjust the port and protocol as needed

    }

    proto::nav_api::ProcessorSettings settings;

    settings.set_detect_bottom(GetBottomDetection());
    double range = GetRangeMeter();
    if (!GetFLSOn()) {
        settings.set_fov((::proto::nav_api::FieldOfView)0);
    }else{
        if (CMath::IsZero(range - 100.0)) {
            settings.set_fov(::proto::nav_api::FieldOfView::k90d100m);
        }
        else if (CMath::IsZero(range - 200)) {
            settings.set_fov(::proto::nav_api::FieldOfView::k90d200m);
        }
        else if (CMath::IsZero(range - 350)) {
            settings.set_fov(::proto::nav_api::FieldOfView::k90d350m);
        }
    } 
    

    settings.set_inwater_squelch(GetInWaterSquelch());
    settings.set_squelchless_inwater_detector(GetAutoSquelch());
    settings.set_min_inwater_squelch(110);
    settings.set_max_inwater_squelch(210);   

    zmq::message_t message;
    ProtoMessageToZeromqMessage__(settings, &message);
    pProcessorSettingsSoc->send(message, zmq::send_flags::none);
}



void CFlsIF::SetRangeMeter(double val)
{
    if (val != (pHdr->FromHostToSim.RangeMeter)) {
        IsHostDataDirty = true;
    }
    pHdr->FromHostToSim.RangeMeter = val;
    pHdr->SonarSimIsUpdateData = true;
}

double CFlsIF::GetRangeMeter()
{
    return pHdr->FromSimToHost.RangeMeter;
}

void CFlsIF::SetFLSOn(bool val)
{
    if (val != (pHdr->FromHostToSim.IsFlsOn?1:0)) {
        IsHostDataDirty = true;
    }
    cout << "Is Fls Standyby" << val << endl;
    pHdr->FromHostToSim.IsFlsOn = val?1:0;
    pHdr->SonarSimIsUpdateData = true;
   
}

bool CFlsIF::GetFLSOn()
{
    return pHdr->FromSimToHost.IsFlsOn;
}

void CFlsIF::SetAutoSquelch(bool val)
{
    if (val != (pHdr->FromHostToSim.IsAutoSquelchEnabled?1:0)) {
        IsHostDataDirty = true;
    }
    pHdr->FromHostToSim.IsAutoSquelchEnabled = val;
    pHdr->SonarSimIsUpdateData = true;
}

bool CFlsIF::GetAutoSquelch()
{
    return pHdr->FromSimToHost.IsAutoSquelchEnabled;
}

void CFlsIF::SetBottomDetection(bool val)
{
    if (val != (pHdr->FromHostToSim.BottomDetectionEnabled ? 1 : 0)) {
        IsHostDataDirty = true;
    }
    pHdr->FromHostToSim.BottomDetectionEnabled = val;
    pHdr->SonarSimIsUpdateData = true;
}

bool CFlsIF::GetBottomDetection()
{
    return pHdr->FromSimToHost.BottomDetectionEnabled;
}

void CFlsIF::SetInWaterSquelch(double val)
{
    if (val != (pHdr->FromHostToSim.SquelchSensitivity)) {
        IsHostDataDirty = true;
    }
    pHdr->FromHostToSim.SquelchSensitivity = val;
    pHdr->SonarSimIsUpdateData = true;
}

double CFlsIF::GetInWaterSquelch()
{
    return pHdr->FromHostToSim.SquelchSensitivity;
}
