// bwtzip_common.cc - bwtzip Common functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "pch.h"
#include "common.hh"
using namespace std;

unsigned char bwtzip::decomposeUL(unsigned long ul, int n) {
    return (ul >> (24 - 8 * n)) & 0xFF;
}

unsigned char bwtzip::decomposeULL(unsigned long long ull, int n) {
    return (ull >> (56 - 8 * n)) & 0xFF;
}

unsigned long long bwtzip::bitsToBytes(unsigned long long bits) {
    return bits / 8 + (bits % 8 != 0);
}

unsigned long bwtzip::readUL(const vector<unsigned char>& v, size_t n) {
    unsigned long ret = 0;
    
    for (int i = 0; i < 4; i++) {
        unsigned long t = v[n + i];
        ret |= t << (24 - 8 * i);
    }

    return ret;
}

unsigned long long bwtzip::readULL(const vector<unsigned char>& v, size_t n) {
    unsigned long long ret = 0;
    
    for (int i = 0; i < 8; i++) {
        unsigned long long t = v[n + i];
        ret |= t << (56 - 8 * i);
    }

    return ret;
}

vector<unsigned char> bwtzip::encodeUL(unsigned long ul) {
    vector<unsigned char> ret;

    for (int i = 0; i < 4; i++) {
        ret.push_back(bwtzip::decomposeUL(ul, i));
    }

    return ret;
}

vector<unsigned char> bwtzip::encodeULL(unsigned long long ull) {
    vector<unsigned char> ret;

    for (int i = 0; i < 8; i++) {
        ret.push_back(bwtzip::decomposeULL(ull, i));
    }

    return ret;
}

bwtzip::PackedBits::PackedBits()
    : waiting(0x00), numwaiting(0) { }

void bwtzip::PackedBits::push_back(bool bit) {
    waiting |= (unsigned char) bit << 7 - numwaiting;
    ++numwaiting;
    if (numwaiting == 8) {
        data.push_back(waiting);
        waiting = 0x00;
        numwaiting = 0;
    }
}

vector<unsigned char> bwtzip::PackedBits::get_vuc() {
    if (numwaiting != 0) {
        data.push_back(waiting);
    }
    return data;
}
