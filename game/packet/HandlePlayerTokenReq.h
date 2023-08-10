#ifndef EYPS_HANDLEPLAYERTOKENREQ_H
#define EYPS_HANDLEPLAYERTOKENREQ_H
#include "PacketHandler.h"
#include "../../resources/proto/GetPlayerTokenReq.pb.h"
#include "../../resources/proto/GetPlayerTokenRsp.pb.h"
#include "../../util/PacketIds.h"
#include "../../util/psConfig.h"
#include "../../util/log.h"
#include "../Kcp.h"
#include "../../util/BasePacket.h"

class Kcp;

class HandlePlayerTokenReq : public PacketHandler{
public:
    HandlePlayerTokenReq() = default;
    ~HandlePlayerTokenReq() override = default;
    void handler(Kcp* client, const BasePacket& packet) override;
};


#endif //EYPS_HANDLEPLAYERTOKENREQ_H
