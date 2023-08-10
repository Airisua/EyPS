#ifndef EYPS_HANDLEPINGREQ_H
#define EYPS_HANDLEPINGREQ_H
#include "PacketHandler.h"
#include "../Kcp.h"
#include "../../util/BasePacket.h"

class Kcp;

class HandlePingReq : public PacketHandler{
public:
    HandlePingReq() = default;
    ~HandlePingReq() override = default;
    void handler(Kcp* client, const BasePacket& packet) override;
};


#endif //EYPS_HANDLEPINGREQ_H
