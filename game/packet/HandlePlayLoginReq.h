#ifndef EYPS_HANDLEPLAYLOGINREQ_H
#define EYPS_HANDLEPLAYLOGINREQ_H
#include "PacketHandler.h"
#include "../../resources/proto/PlayerLoginReq.pb.h"
#include "../../resources/proto/PlayerLoginRsp.pb.h"
#include "../../resources/proto/DoSetPlayerBornDataNotify.pb.h"
#include "../../resources/proto/OpenStateUpdateNotify.pb.h"
#include "../../resources/proto/StoreWeightLimitNotify.pb.h"
#include "../../resources/proto/PlayerStoreNotify.pb.h"
#include "../../resources/proto/AvatarDataNotify.pb.h"
#include "../../resources/proto/PlayerEnterSceneNotify.pb.h"
#include "../Kcp.h"
#include "../../util/BasePacket.h"

#include <boost/beast/core/detail/base64.hpp>
#include <boost/format.hpp>
#include <iostream>

class Kcp;

namespace base64 = boost::beast::detail::base64;

class HandlePlayLoginReq : public PacketHandler{
public:
    HandlePlayLoginReq() = default;
    ~HandlePlayLoginReq() override = default;
    void handler(Kcp* client, const BasePacket& packet) override;
};


#endif //EYPS_HANDLEPLAYLOGINREQ_H
