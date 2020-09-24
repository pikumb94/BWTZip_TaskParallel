// bwtzip_zle.hh - bwtzip Zero Length Encoder functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// The Wheeler Zero Length Encoder is a clever algorithm
// developed by D.J. Wheeler. It remains unpublished.

// Cleartext: A run of N 0x00 bytes
// Encoded as: N + 1 is written down as a big-endian binary number
// with the most significant bit elided.
// Example:
// N:                              1, 2, 3,  4,  5,  6,  7,   8,   9, ...
// N + 1 in binary and MSB elided: 0, 1, 00, 01, 10, 11, 000, 001, 010, ...
// 0 bits are encoded as 0x00 and 1 bits are encoded as 0x01.

// Cleartext: A byte B = 0x01 or 0x02 or ... or 0xFD
// Encoded as: B + 1

// Cleartext: A 0xFE byte
// Encoded as: 0xFF 0x00

// Cleartext: A 0xFF byte
// Encoded as: 0xFF 0x01

#ifndef ZLE_HH
#define ZLE_HH

#include <vector>

using namespace std;

void ZLE(string &in);

namespace bwtzip {
    std::vector<unsigned char>& zleWheeler(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unzleWheeler(std::vector<unsigned char>& v);
}


#endif
