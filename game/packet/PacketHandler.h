#ifndef EYPS_PACKETHANDLER_H
#define EYPS_PACKETHANDLER_H
#include "../Kcp.h"
#include "../../util/BasePacket.h"
#include <memory>

class Kcp;

class PacketHandler{
public:
    PacketHandler() =default;
    virtual ~PacketHandler() = default;
    virtual void handler(Kcp* client, const BasePacket& packet) = 0;
};


#endif //EYPS_PACKETHANDLER_H
