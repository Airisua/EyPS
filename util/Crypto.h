#ifndef EYPS_CRYPTO_H
#define EYPS_CRYPTO_H

#include <array>
#include <span>
#include <random>
#include <string_view>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/asio/ssl.hpp>

std::span<uint8_t> Xor(std::span<uint8_t> packet, std::span<uint8_t> key);
std::span<uint8_t> Xor(std::vector<uint8_t> packet, std::span<uint8_t> key);
std::span<uint8_t> Xor(std::string_view packet, std::span<uint8_t> key);
std::string_view Xor(std::string& packet, std::string_view key);
std::span<uint8_t> Xor(uint8_t *packet, std::span<uint8_t> key,size_t len);
std::vector<uint8_t> generateKey(unsigned long long seed);

//std::string_view ReadFile_(const char* filename);
std::span<uint8_t> ReadFile_(const char* filename);

bool Base64Encode(uint8_t & input,std::string &output,size_t len);
bool Base64Encode(const std::string& input,std::string &output);
bool Base64Decode(const std::string& input, std::string &output);

void load_certificate(boost::asio::ssl::context& ctx);
std::string createToken();


std::string RsaPubDecrypt(const std::string & cipher_text);
std::string DecryptByPrikeyFile(std::string& cipher);
#endif //EYPS_CRYPTO_H
