// bwtzip_zle.cc - bwtzip Zero Length Encoder functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "pch.h"
#include "ZLE.hh"
#include <algorithm>
using namespace std;

void ZLE(string &in) {
	vector <unsigned char> v;
	v.assign(in.begin(), in.end());
	bwtzip::zleWheeler(v);
	in.assign(v.begin(), v.end());
}

namespace {
    void encodeLengthWheeler(vector<unsigned char>& v, size_t length) {
        length++;                           
    
        vector<unsigned char> temp;
    
        while (length != 1) {               
            temp.push_back(length & 1);
            length >>= 1;
        }

        reverse(temp.begin(), temp.end());
    
        v.insert(v.end(), temp.begin(), temp.end());
    }
}

vector<unsigned char>& bwtzip::zleWheeler(vector<unsigned char>& v) {
    vector<unsigned char> ret;
    size_t length = 0;

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char byte = v[i];

        if (byte == 0x00) {
            length++;
        } else if (length != 0) {
            encodeLengthWheeler(ret, length);
            length = 0;
        }

        if (byte == 0xFE) {
            ret.push_back(0xFF);
            ret.push_back(0x00);
        } else if (byte == 0xFF) {
            ret.push_back(0xFF);
            ret.push_back(0x01);
        } else if (byte != 0x00) {
            ret.push_back(byte + 1);
        }
    }

    if (length != 0) {
        encodeLengthWheeler(ret, length);
    }

    swap(ret, v);
    return v;
}

vector<unsigned char>& bwtzip::unzleWheeler(vector<unsigned char>& v) {
    vector<unsigned char> ret;

    size_t length = 0;
    bool   run = false;

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char byte = v[i];

        if (byte <= 0x01) {
            if (run == false) { 
                length = 1;
                run = true;
            }

            length <<= 1;
            length += byte;
        } else if (run) {
            run = false;
            length--;  
    
            while (length != 0) {
                ret.push_back(0x00);
                length--;
            }
        }
        
        if (byte == 0xFF) {
            const unsigned char nextbyte = v[++i];

            if (nextbyte == 0x00) {
                ret.push_back(0xFE);
            } else {                    
                ret.push_back(0xFF);
            }
        } else if (byte > 0x01) {
            ret.push_back(byte - 1);
        }
    }

    if (run) {
        length--;

        while (length != 0) {
            ret.push_back(0x00);
            length--;
        }
    }

    swap(ret, v);
    return v;
}
