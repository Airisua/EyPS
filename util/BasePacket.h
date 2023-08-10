#ifndef EYPS_BASEPACKET_H
#define EYPS_BASEPACKET_H

#include "bufferView.h"
#include "../resources/proto/PacketHead.pb.h"
#include "PacketIds.h"

class BasePacket : public bufferView{
public:
    BasePacket(const std::span<const std::uint8_t> data);

    BasePacket(const std::span<const std::uint8_t> data, const short packet_id, const int client_sequence = 0);

    BasePacket(const std::string& data, const short packet_id, const int client_sequence = 0);

    std::uint16_t m_MagicStart;
    std::uint16_t m_PacketId{};
    std::uint16_t m_MetadataSize{};
    std::uint32_t m_DataSize{};
    PacketHead m_Metadata;
    std::span<const uint8_t> m_Data;
    std::uint16_t m_MagicEnd;
};


#endif //EYPS_BASEPACKET_H
