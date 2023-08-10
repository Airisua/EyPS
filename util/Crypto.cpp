#include "Crypto.h"
#include "dynamicBuffer.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>

std::span<uint8_t> Xor(std::span<uint8_t> packet, std::span<uint8_t> key) {
    auto packet_size = packet.size();
    auto key_size = key.size();

    auto *buf = new uint8_t [packet_size];
    memcpy(buf, packet.data(), packet_size);

    for (int i = 0; i < packet_size; i++){
        buf[i] ^= key[i % key_size];
    }
    return { (uint8_t*)buf, (size_t)packet_size };
}

std::span<uint8_t> Xor(std::vector<uint8_t> packet, std::span<uint8_t> key) {
    auto packet_size = packet.size();
    auto key_size = key.size();
    auto *buf = new uint8_t [packet_size];
    memcpy(buf, packet.data(), packet_size);

    for (int i = 0; i < packet_size; i++){
        buf[i] ^= key[i % key_size];
    }
    return { (uint8_t*)buf, (size_t)packet_size };
}

std::span<uint8_t> Xor( std::string_view packet, std::span<uint8_t> key) {
    auto packet_size = packet.size();
    auto key_size = key.size();

    auto *buf  = new uint8_t [packet_size];
    memcpy(buf, packet.data(), packet_size);

    for (int i = 0; i < packet_size; ++i){
        buf[i] ^= key[i % key_size];
    }

    return { (uint8_t*)buf, (size_t)packet_size };
}

std::string_view Xor(std::string& packet, std::string_view key) {
    auto packet_size = packet.size();
    auto key_size = key.size();

    for(int i = 0; i < packet_size; ++i)
        packet[i] ^= key[i % key_size];

    return packet;
}

std::span<uint8_t> Xor(uint8_t *packet, std::span<uint8_t> key,size_t packet_size) {
    uint8_t buf [packet_size];
    memcpy(buf, &packet, packet_size);

    auto key_size = key.size();
    for(int i = 0; i < packet_size; ++i)
        packet[i] ^= key[i % key_size];

    return { (uint8_t*)buf, (size_t)packet_size };
}


std::vector<uint8_t> generateKey(unsigned long long seed) {
    std::mt19937_64 generator;

    generator.seed(seed);
    generator.seed(generator());
    generator();
    dynamicBuffer buf;

    for (int i = 0; i < 4096; i += 8){
        buf.Write<uint64_t>(generator(), true);
    }
    return buf.GetDataOwnership();
}


std::span<uint8_t> ReadFile_(const char* filename) {
    long long size;
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    file.ignore(std::numeric_limits<std::streamsize>::max());

    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);

    auto buf = (uint8_t*)malloc(size);

    file.read((char*)buf, size);
    file.close();


    return { buf, (size_t)size };
}

bool Base64Encode(const std::string& input,std::string &output) {
    auto len = input.size();
    output.resize(boost::beast::detail::base64::encoded_size(len));
    output.resize(boost::beast::detail::base64::encode(output.data(), input.c_str(), len));
    return true;
}

bool Base64Encode(uint8_t & input,std::string &output,size_t len) {

    output.resize(boost::beast::detail::base64::encoded_size(len));
    output.resize(boost::beast::detail::base64::encode(output.data(), &input, len));
    return true;
}

bool Base64Decode(const std::string& input, std::string &output) {
    auto len = input.size();
    output.resize(boost::beast::detail::base64::decoded_size(len));
    auto result = boost::beast::detail::base64::decode(&output[0], input.data(), len);
    output.resize(result.first);
    return true;
}



void load_certificate(boost::asio::ssl::context& ctx) {
    ctx.set_options(
            boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::no_sslv2);

    ctx.use_certificate_file("../resources/cert/cert.crt",boost::asio::ssl::context::file_format::pem);
    ctx.use_private_key_file("../resources/cert/private.key",boost::asio::ssl::context::file_format::pem);

}

std::string createToken() {
    auto seed_ = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 g1(seed_);
    g1.seed(seed_);
    auto  u32Rand = g1();

    auto token = std::to_string(u32Rand);

    unsigned char digest[20];
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, token.c_str(), strlen(token.c_str()));
    SHA1_Final(digest, &ctx);

    char mdString[20*2+1];
    for (int i = 0; i < 20; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    return mdString;
}













// test
std::string DecryptByPrikeyFile(std::string& cipher) {

    const char* filename = "../resources/rsa/auth_private-key.pem";
    long long sz;
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    file.ignore(std::numeric_limits<std::streamsize>::max());

    file.seekg(0, std::ios::end);
    sz = file.tellg();
    file.seekg(0, std::ios::beg);

    auto buf = (char *)malloc(sz);

    file.read((char*)buf, sz);
    file.close();


//#########################################
    std::string b{};
    Base64Decode(buf,b);

    std::cout << "private key: " << b << std::endl;


    std::string result;

    int key_len = RSA_size(reinterpret_cast<const RSA *>(b.c_str()));
    char *decrypt_data = new char[key_len + 1];
    memset(decrypt_data, 0, key_len + 1);

    int len = RSA_private_decrypt(cipher.size(), (const unsigned char *)cipher.c_str(),
                                  (unsigned char *)decrypt_data, (RSA *) b.c_str(), RSA_PKCS1_PADDING);

    if (len == -1) {
        // 解密失败
        ERR_load_crypto_strings();
        char errbuf[256] = {0};
        ERR_error_string(ERR_get_error(), errbuf);
        printf("decrypt err %s\n", errbuf);

        delete[] decrypt_data;
        return result;
    }

    result.append(decrypt_data, len);
    delete[] decrypt_data;

    return result;
}
