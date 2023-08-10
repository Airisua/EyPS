#ifndef EYPS_PACKETMANAGER_H
#define EYPS_PACKETMANAGER_H
#include "PacketHandler.h"
#include "HandlePingReq.h"
#include "HandlePlayerTokenReq.h"
#include "HandlePlayLoginReq.h"
#include "test.h"
#include "../../util/Singleton.h"
#include "../../util/PacketIds.h"
#include <memory>
#include <unordered_map>


class packetManager : public Singleton<packetManager>{
    friend class Singleton<packetManager>;
    packetManager()=default;
public:
    ~packetManager() override = default;
    void init();

};

std::unordered_map<int,std::shared_ptr<PacketHandler>> m_Packets;

void packetManager::init() {
    m_Packets.insert({
        {PacketIds::PingReq,std::make_shared<HandlePingReq>()},
        {PacketIds::GetPlayerTokenReq,std::make_shared<HandlePlayerTokenReq>()},
        {PacketIds::PlayerLoginReq,std::make_shared<HandlePlayLoginReq>()}
    });
}

#endif //EYPS_PACKETMANAGER_H
