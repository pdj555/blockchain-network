#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace crypto {

namespace detail {

inline uint32_t rotr(uint32_t x, uint32_t n) { return (x >> n) | (x << (32U - n)); }

inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (~x & z); }
inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }

inline uint32_t bigSigma0(uint32_t x) { return rotr(x, 2U) ^ rotr(x, 13U) ^ rotr(x, 22U); }
inline uint32_t bigSigma1(uint32_t x) { return rotr(x, 6U) ^ rotr(x, 11U) ^ rotr(x, 25U); }
inline uint32_t smallSigma0(uint32_t x) { return rotr(x, 7U) ^ rotr(x, 18U) ^ (x >> 3U); }
inline uint32_t smallSigma1(uint32_t x) { return rotr(x, 17U) ^ rotr(x, 19U) ^ (x >> 10U); }

inline uint32_t load_be_u32(const uint8_t *p) {
    return (static_cast<uint32_t>(p[0]) << 24U) | (static_cast<uint32_t>(p[1]) << 16U) |
           (static_cast<uint32_t>(p[2]) << 8U) | static_cast<uint32_t>(p[3]);
}

inline void store_be_u32(uint8_t *out, uint32_t v) {
    out[0] = static_cast<uint8_t>((v >> 24U) & 0xFFU);
    out[1] = static_cast<uint8_t>((v >> 16U) & 0xFFU);
    out[2] = static_cast<uint8_t>((v >> 8U) & 0xFFU);
    out[3] = static_cast<uint8_t>(v & 0xFFU);
}

inline void store_be_u64(uint8_t *out, uint64_t v) {
    out[0] = static_cast<uint8_t>((v >> 56U) & 0xFFU);
    out[1] = static_cast<uint8_t>((v >> 48U) & 0xFFU);
    out[2] = static_cast<uint8_t>((v >> 40U) & 0xFFU);
    out[3] = static_cast<uint8_t>((v >> 32U) & 0xFFU);
    out[4] = static_cast<uint8_t>((v >> 24U) & 0xFFU);
    out[5] = static_cast<uint8_t>((v >> 16U) & 0xFFU);
    out[6] = static_cast<uint8_t>((v >> 8U) & 0xFFU);
    out[7] = static_cast<uint8_t>(v & 0xFFU);
}

inline const std::array<uint32_t, 64> &k() {
    static const std::array<uint32_t, 64> constants = {
        0x428a2f98U, 0x71374491U, 0xb5c0fbcfU, 0xe9b5dba5U, 0x3956c25bU, 0x59f111f1U,
        0x923f82a4U, 0xab1c5ed5U, 0xd807aa98U, 0x12835b01U, 0x243185beU, 0x550c7dc3U,
        0x72be5d74U, 0x80deb1feU, 0x9bdc06a7U, 0xc19bf174U, 0xe49b69c1U, 0xefbe4786U,
        0x0fc19dc6U, 0x240ca1ccU, 0x2de92c6fU, 0x4a7484aaU, 0x5cb0a9dcU, 0x76f988daU,
        0x983e5152U, 0xa831c66dU, 0xb00327c8U, 0xbf597fc7U, 0xc6e00bf3U, 0xd5a79147U,
        0x06ca6351U, 0x14292967U, 0x27b70a85U, 0x2e1b2138U, 0x4d2c6dfcU, 0x53380d13U,
        0x650a7354U, 0x766a0abbU, 0x81c2c92eU, 0x92722c85U, 0xa2bfe8a1U, 0xa81a664bU,
        0xc24b8b70U, 0xc76c51a3U, 0xd192e819U, 0xd6990624U, 0xf40e3585U, 0x106aa070U,
        0x19a4c116U, 0x1e376c08U, 0x2748774cU, 0x34b0bcb5U, 0x391c0cb3U, 0x4ed8aa4aU,
        0x5b9cca4fU, 0x682e6ff3U, 0x748f82eeU, 0x78a5636fU, 0x84c87814U, 0x8cc70208U,
        0x90befffaU, 0xa4506cebU, 0xbef9a3f7U, 0xc67178f2U};
    return constants;
}

inline std::array<uint8_t, 32> sha256_bytes(const std::vector<uint8_t> &message) {
    std::array<uint32_t, 8> h = {0x6a09e667U, 0xbb67ae85U, 0x3c6ef372U, 0xa54ff53aU,
                                 0x510e527fU, 0x9b05688cU, 0x1f83d9abU, 0x5be0cd19U};

    std::array<uint32_t, 64> w{};
    for (std::size_t offset = 0; offset < message.size(); offset += 64U) {
        const uint8_t *chunk = message.data() + offset;
        for (std::size_t i = 0; i < 16U; ++i) {
            w[i] = load_be_u32(chunk + (i * 4U));
        }
        for (std::size_t i = 16U; i < 64U; ++i) {
            w[i] = smallSigma1(w[i - 2U]) + w[i - 7U] + smallSigma0(w[i - 15U]) + w[i - 16U];
        }

        uint32_t a = h[0];
        uint32_t b = h[1];
        uint32_t c = h[2];
        uint32_t d = h[3];
        uint32_t e = h[4];
        uint32_t f = h[5];
        uint32_t g = h[6];
        uint32_t hh = h[7];

        for (std::size_t i = 0; i < 64U; ++i) {
            const uint32_t t1 = hh + bigSigma1(e) + ch(e, f, g) + k()[i] + w[i];
            const uint32_t t2 = bigSigma0(a) + maj(a, b, c);
            hh = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += hh;
    }

    std::array<uint8_t, 32> digest{};
    for (std::size_t i = 0; i < 8U; ++i) {
        store_be_u32(digest.data() + (i * 4U), h[i]);
    }
    return digest;
}

} // namespace detail

inline std::string sha256_hex(const std::string &input) {
    std::vector<uint8_t> msg(input.begin(), input.end());
    const uint64_t bitLen = static_cast<uint64_t>(msg.size()) * 8ULL;

    msg.push_back(0x80U);
    while ((msg.size() % 64U) != 56U) {
        msg.push_back(0x00U);
    }

    std::array<uint8_t, 8> lenBytes{};
    detail::store_be_u64(lenBytes.data(), bitLen);
    msg.insert(msg.end(), lenBytes.begin(), lenBytes.end());

    const std::array<uint8_t, 32> digest = detail::sha256_bytes(msg);

    static const char *hex = "0123456789abcdef";
    std::string out;
    out.reserve(64);
    for (uint8_t b : digest) {
        out.push_back(hex[(b >> 4U) & 0x0FU]);
        out.push_back(hex[b & 0x0FU]);
    }
    return out;
}

} // namespace crypto

