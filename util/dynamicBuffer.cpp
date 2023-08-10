#include "dynamicBuffer.h"
#include <numeric>
#include <stdexcept>
#include <cstring>


dynamicBuffer::dynamicBuffer(std::size_t initialSize) :m_CurOffset(0) {
    this->m_Buffer.reserve(initialSize);
}

void dynamicBuffer::WriteString(std::string_view str) {
    if (str.size() >= std::numeric_limits<std::uint8_t>::max()){
        throw std::length_error("String to write is too large");
    }

    auto length = str.size();
    this->Write(std::uint8_t(length));

    auto srcPtr = reinterpret_cast<const std::uint8_t*>(str.data());
    this->WriteArray(std::span<const std::uint8_t>{ srcPtr, str.size() });
}

void dynamicBuffer::WriteLongString(std::string_view str) {
    if (str.size() >= std::numeric_limits<std::uint16_t>::max()){
        throw std::length_error("String to write is too large");
    }

    auto length = str.size();
    this->Write(std::uint16_t(length));

    auto srcPtr = reinterpret_cast<const std::uint8_t*>(str.data());
    this->WriteArray(std::span<const std::uint8_t>{ srcPtr, str.size() });
}

void dynamicBuffer::WriteArray(std::span<const std::uint8_t> data) {
    auto predictedBufSize = this->m_CurOffset + data.size_bytes();

    if (predictedBufSize >= this->m_Buffer.capacity()){
        auto sizeAvail = predictedBufSize - this->m_Buffer.capacity();
        this->GrowBuffer(sizeAvail);
    }

    this->m_Buffer.insert(this->m_Buffer.end(), data.begin(), data.end());
    this->m_CurOffset += data.size_bytes();
}

void dynamicBuffer::WriteToOffsetArray(std::span<const std::uint8_t> data, std::size_t offset) {
    if (offset >= this->m_Buffer.size() || offset + data.size_bytes() >= this->m_Buffer.size() || offset >= this->m_Buffer.capacity() || offset + data.size_bytes() >= this->m_Buffer.capacity()){
        throw std::runtime_error("Offset is out of buffer bounds");
    }

    std::memcpy(this->m_Buffer.data() + offset, data.data(), data.size_bytes());
}

void dynamicBuffer::GrowBuffer(std::size_t bytesToGrow) {
    auto newSize = this->m_Buffer.capacity() + bytesToGrow;
    this->m_Buffer.reserve(newSize);
}

void dynamicBuffer::Seek(std::size_t offset) {
    this->m_CurOffset = offset;
}

std::vector<std::uint8_t> &&dynamicBuffer::GetDataOwnership() {
    return std::move(this->m_Buffer);
}