#include "HandlePingReq.h"
#include "../../util/Crypto.h"
//#include "../../util/log.h"
#include "../../resources/proto/PingReq.pb.h"
#include "../../resources/proto/PingRsp.pb.h"

void HandlePingReq::handler(Kcp *client, const BasePacket &packet) {
    auto data = packet.m_Data;

    PingReq ping_req;
    ping_req.ParseFromArray(data.data(), data.size());

    PingRsp ping_rsp;
    ping_rsp.set_client_time(ping_req.client_time());

    BasePacket ping_rsp_packet(ping_rsp.SerializeAsString(),PacketIds::PingRsp,packet.m_Metadata.client_sequence_id());
    client->Send(ping_rsp_packet);
    std::cout << "ping req...\n";
}
