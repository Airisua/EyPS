#include "HandlePlayerTokenReq.h"

void HandlePlayerTokenReq::handler(Kcp *client, const BasePacket &packet) {
    auto fig = Config::getInst();
    auto data = packet.m_Data;
    LOG_DEBUG("player token req");
    GetPlayerTokenReq TokenReq;
    TokenReq.ParseFromArray(data.data(),data.size());

    unsigned long long secret_key_seed = 0x123456789ABCDEF0;
    unsigned long long seed = 0;

    printf("[GameServer::ParsePacket] GetPlayerTokenReq from %s:%d, UID: %s, Account Token:%s\n",
           client->endpoint.address().to_string().c_str(),
           client->endpoint.port(),
           TokenReq.account_uid().c_str(),
           TokenReq.account_token().c_str());

    GetPlayerTokenRsp TokenRsp;
    TokenRsp.set_uid(TokenReq.uid());
    TokenRsp.set_account_uid(TokenReq.account_uid());
    TokenRsp.set_token(TokenReq.account_token());
    TokenRsp.set_account_type(TokenReq.account_type());

    TokenRsp.set_secret_key_seed(secret_key_seed);

    TokenRsp.set_is_proficient_player(true);
    TokenRsp.set_security_cmd_buffer((char *)fig->getSecretKeyBuffer_().data());
    TokenRsp.set_platform_type(3);
    TokenRsp.set_channel_id(1);
    TokenRsp.set_country_code("***");
    TokenRsp.set_reg_platform(3);
    TokenRsp.set_client_ip_str(client->endpoint.address().to_string());
    printf("TokenRsp: packet:%s bt size: %llu\n", TokenRsp.SerializeAsString().c_str(),TokenRsp.ByteSizeLong());
    client->SetUseMT(true);
    client->GenerateMTKey(secret_key_seed);
    client->sessionState = Kcp::SessionState::WAITING_FOR_LOGIN;

    BasePacket TokenRspPacket(TokenRsp.SerializeAsString(), PacketIds::GetPlayerTokenRsp);

    client->Send(TokenRspPacket, true);
}
