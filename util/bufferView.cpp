#include <stdexcept>
#include "bufferView.h"

bufferView::bufferView(const std::span<const std::uint8_t> data) : m_DataView(data), m_CurDataOffset(0){}

std::string bufferView::ReadString() {
    std::size_t length = this->Read<std::uint8_t>();

    if (!this->CanReadBytes(length)){
        throw std::length_error(
                "The string's length is larger than the available data");
    }

    std::string newStr;

    if (length > 0){
        auto strStart = reinterpret_cast<const char*>(
                &this->m_DataView[this->m_CurDataOffset]);

        newStr.reserve(length + 1);
        newStr.append(strStart, length);

        this->m_CurDataOffset += length;
    }

    return newStr;
}

std::string bufferView::ReadLongString() {
    std::size_t length = this->Read<std::uint16_t>();

    if (!this->CanReadBytes(length)){
        throw std::length_error(
                "The string's length is larger than the available data");
    }

    std::string newStr;

    if (length > 0){
        auto strStart = reinterpret_cast<const char*>(
                &this->m_DataView[this->m_CurDataOffset]);

        newStr.reserve(length + 1);
        newStr.append(strStart, length);

        this->m_CurDataOffset += length;
    }

    return newStr;
}

std::span<const uint8_t> bufferView::ReadView(std::size_t viewSize) {
    if (!this->CanReadBytes(viewSize)){
        throw std::length_error(
                "The desired data's length is larger than the available data");
    }

    auto dataStart = reinterpret_cast<const std::uint8_t*>(
            &this->m_DataView[this->m_CurDataOffset]);

    auto res = std::span<const uint8_t>{ dataStart, viewSize };

    this->m_CurDataOffset += viewSize;
    return res;
}

void bufferView::ReadImpl(std::span<std::uint8_t> outData) {
    if (!this->CanReadBytes(outData.size_bytes())){
        throw std::length_error(
                "The desired data's length is larger than the available data");
    }

    auto dataStart = reinterpret_cast<const std::uint8_t*>(
            &this->m_DataView[this->m_CurDataOffset]);

    std::copy(dataStart, dataStart + outData.size_bytes(), outData.begin());
    this->m_CurDataOffset += outData.size_bytes();
}

void bufferView::WriteArray(std::span<const std::uint8_t> data) {
    auto predictedBufSize = this->m_CurDataOffset + data.size_bytes();

    if (predictedBufSize >= this->m_Buffer.capacity()){
        auto sizeAvail = predictedBufSize - this->m_Buffer.capacity();
        this->GrowBuffer(sizeAvail);
    }

    this->m_Buffer.insert(this->m_Buffer.end(), data.begin(), data.end());
    this->m_CurDataOffset += data.size_bytes();
}

void bufferView::GrowBuffer(std::size_t bytesToGrow) {
    auto newSize = this->m_Buffer.capacity() + bytesToGrow;
    this->m_Buffer.reserve(newSize);
}

std::vector<std::uint8_t> &&bufferView::GetDataOwnership() {
    return std::move(this->m_Buffer);
}
