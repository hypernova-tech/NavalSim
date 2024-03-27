#include "CFlsIF.h"
#include <iostream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "zeromq/include/zmq.h"
#include "zeromq/include/zmq.hpp"

#include "proto/nav_api.pb.h"
void CFlsIF::UpdateFlsData(SSharedMemBufferHdr hdr, SFLSDataEntry* p_entries, INT32U count)
{

    if (count <= 0) {
        return;
    }

    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:61502"); // Adjust the port and protocol as needed

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

    // Serialize the TargetData message
    std::string serializedData;
    if (!targetData.SerializeToString(&serializedData)) {
        std::cerr << "Failed to serialize TargetData." << std::endl;
        return;
    }

    // Publish the serialized TargetData
    zmq::message_t message(serializedData.begin(), serializedData.end());
    publisher.send(message, zmq::send_flags::none);


}
