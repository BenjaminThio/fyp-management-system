#pragma once
#include <random>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

class UUIDv4 {
public:
    static string generate() {
        static thread_local random_device rd;
        static thread_local mt19937_64 gen(rd());
        static thread_local uniform_int_distribution<uint64_t> dis;

        uint64_t high = dis(gen);
        uint64_t low  = dis(gen);

        high &= 0xFFFFFFFFFFFF0FFFULL;
        high |= 0x0000000000004000ULL;

        low &= 0x3FFFFFFFFFFFFFFFULL;
        low |= 0x8000000000000000ULL;

        stringstream ss;
        ss << hex << setfill('0');

        ss << setw(8) << static_cast<uint32_t>(high >> 32);
        ss << '-';
        ss << setw(4) << static_cast<uint16_t>((high >> 16) & 0xFFFF);
        ss << '-';
        ss << setw(4) << static_cast<uint16_t>(high & 0xFFFF);
        ss << '-';
        ss << setw(4) << static_cast<uint16_t>(low >> 48);
        ss << '-';
        ss << setw(4) << static_cast<uint16_t>((low >> 32) & 0xFFFF);
        ss << setw(8) << static_cast<uint32_t>(low & 0xFFFFFFFF);

        return ss.str();
    }
};