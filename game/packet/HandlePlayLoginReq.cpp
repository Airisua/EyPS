#include "HandlePlayLoginReq.h"
#include "../../resources/proto/PlayerDataNotify.pb.h"

void HandlePlayLoginReq::handler(Kcp *client, const BasePacket &packet) {
    // test
    auto data = packet.m_Data;
    auto uid = 10001;
    PlayerLoginReq LoginReq;
    LoginReq.ParseFromArray(data.data(), data.size());


    OpenStateUpdateNotify states;
    for(int i = 1; i < 2602; i++)
        (*states.mutable_open_state_map())[i] = 1;

    BasePacket states_packet(states.SerializeAsString(), PacketIds::OpenStateUpdateNotify);
    client->Send(states_packet);

    StoreWeightLimitNotify storage;
    storage.set_material_count_limit(2000);
    storage.set_weight_limit(30000);
    storage.set_store_type(StoreType::STORE_TYPE_PACK);
    storage.set_weapon_count_limit(2000);
    storage.set_reliquary_count_limit(2000);
    storage.set_furniture_count_limit(2000);

    BasePacket storageweightpacket(storage.SerializeAsString(), PacketIds::StoreWeightLimitNotify);
    client->Send(storageweightpacket);

    PlayerStoreNotify player_store;

    auto item = player_store.add_item_list();
    item->set_item_id(11101);
    item->set_guid(42992622632962);
    item->mutable_equip()->mutable_weapon()->set_level(1);

    player_store.set_store_type(StoreType::STORE_TYPE_PACK);
    player_store.set_weight_limit(30000);

    BasePacket player_store_packet(player_store.SerializeAsString(), PacketIds::PlayerStoreNotify);
    client->Send(player_store_packet);

    AvatarDataNotify avatardata;
    const char* avatar_data_b64 = "";

    char* tmp_buf = new char[2000];
    auto lol = base64::decode(tmp_buf, avatar_data_b64, 1677);
    avatardata.ParseFromArray(tmp_buf, lol.first);

    BasePacket avatar_data_packet(avatardata.SerializeAsString(), PacketIds::AvatarDataNotify);
    client->Send(avatar_data_packet);
    delete[] tmp_buf;


    PlayerEnterSceneNotify playerscenenotify;
    playerscenenotify.set_type(EnterType::ENTER_TYPE_SELF);
    playerscenenotify.set_target_uid(uid);
    playerscenenotify.set_enter_scene_token(123);
    playerscenenotify.set_is_first_login_enter_scene(true);
    playerscenenotify.set_scene_begin_time(time(0));
    playerscenenotify.set_scene_id(3);

    auto vec = playerscenenotify.mutable_pos();

    vec->set_x(2747);
    vec->set_y(194);
    vec->set_z(-1719);

    playerscenenotify.set_world_type(1);
    playerscenenotify.set_enter_reason(1);

    BasePacket player_scene_notify_packet(playerscenenotify.SerializeAsString(), PacketIds::PlayerEnterSceneNotify);
    client->Send(player_scene_notify_packet);

    PlayerLoginRsp LoginRsp;
    LoginRsp.set_is_use_ability_hash(true);
    LoginRsp.set_ability_hash_code(123456);
    LoginRsp.set_game_biz("");
    LoginRsp.set_is_sc_open(false);
    LoginRsp.set_country_code("****");
    client->sessionState = Kcp::SessionState::PICKING_CHARACTER;

    BasePacket TokenRspPacket(LoginRsp.SerializeAsString(), PacketIds::GetPlayerTokenRsp);

    client->Send(TokenRspPacket, true);
}
