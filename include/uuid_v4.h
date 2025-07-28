#pragma once
#include <random>
#include <sstream>
#include <iomanip>

class UUIDv4 {
public:
    static std::string generate() {
        static thread_local std::random_device rd;
        static thread_local std::mt19937_64 gen(rd());
        static thread_local std::uniform_int_distribution<uint64_t> dis;

        uint64_t high = dis(gen);
        uint64_t low  = dis(gen);

        high &= 0xFFFFFFFFFFFF0FFFULL;
        high |= 0x0000000000004000ULL;

        low &= 0x3FFFFFFFFFFFFFFFULL;
        low |= 0x8000000000000000ULL;

        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        ss << std::setw(8) << static_cast<uint32_t>(high >> 32);
        ss << '-';
        ss << std::setw(4) << static_cast<uint16_t>((high >> 16) & 0xFFFF);
        ss << '-';
        ss << std::setw(4) << static_cast<uint16_t>(high & 0xFFFF);
        ss << '-';
        ss << std::setw(4) << static_cast<uint16_t>(low >> 48);
        ss << '-';
        ss << std::setw(4) << static_cast<uint16_t>((low >> 32) & 0xFFFF);
        ss << std::setw(8) << static_cast<uint32_t>(low & 0xFFFFFFFF);

        return ss.str();
    }
};