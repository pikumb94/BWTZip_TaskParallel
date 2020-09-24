// bwtzip_arith.cc - bwtzip Adaptive Arithmetic Coding functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "pch.h"
#include "ARI.hh"
#include <cstdlib>
using namespace std;
using namespace bwtzip;
using namespace bwtzip::arithmetic;



// Do not do ANYTHING with an ArithEncoder after finalizing it,
// except for destruction!

string ARI(string in) {
	string out;

	vector <unsigned char> v;
	v.assign(in.begin(), in.end());
	bwtzip::arith(v);
	out.assign(v.begin(), v.end());

	return  out;
}

bwtzip::arithmetic::ArithModel::ArithModel()
    : freq(NUM_SYMBOLS, 1), cfreq(NUM_SYMBOLS + 1) {

    for (size_t j = 0; j <= NUM_SYMBOLS; j++) {
        cfreq[j] = NUM_SYMBOLS - j;
    }
}

const size_t& bwtzip::arithmetic::ArithModel::operator[](size_t i) const {
    return cfreq[i];
}

void bwtzip::arithmetic::ArithModel::update(int sym) {
    if (cfreq[0] >= MAX_FREQUENCY) {
        int cum = 0;
        cfreq[NUM_SYMBOLS] = 0;
        for (int i = NUM_SYMBOLS - 1; i >= 0; i--) {
            freq[i] = (freq[i] + 1) / 2;
            cum += freq[i];
            cfreq[i] = cum;
        }
    }

    freq[sym] += INCREMENT_VALUE;
    for (int i = sym; i >= 0; i--) {
        cfreq[i] += INCREMENT_VALUE;
    }
}

bwtzip::arithmetic::ArithEncoder::ArithEncoder()
    : low(0), high(TOP_VALUE), fbits(0) { }

void bwtzip::arithmetic::ArithEncoder::encodeSymbol(int sym) {
    unsigned long int range = high - low + 1;
    high = low + (range * acm[sym]) / acm[0] - 1;
    low  = low + (range * acm[sym + 1]) / acm[0];

    for (;;) {
        if (high < HALF) {
            bitPlusFollow(0);
        } else if (low >= HALF) {
            bitPlusFollow(1);
            low -= HALF;
            high -= HALF;
        } else if (low >= FIRST_QTR && high < THIRD_QTR) {
            fbits++;
            low -= FIRST_QTR;
            high -= FIRST_QTR;
        } else {
            break;
        }

        low = 2 * low;
        high = 2 * high + 1;
    }

    acm.update(sym);
}

vector<unsigned char> bwtzip::arithmetic::ArithEncoder::finalize() {
    fbits++;

    if (low < FIRST_QTR) {
        bitPlusFollow(0);
    } else {
        bitPlusFollow(1);
    }

    return output.get_vuc();
}

void bwtzip::arithmetic::ArithEncoder::bitPlusFollow(bool bit) {
    output.push_back(bit);
    while (fbits > 0)  {
        output.push_back(!bit);
        fbits--;
    }
}

bwtzip::arithmetic::ArithDecoder::ArithDecoder(vector<unsigned char> *pv)
    : pvec(pv), loc(0), value(0), low(0), high(TOP_VALUE) {

    for (unsigned long i = 0; i < CODE_VALUE_BITS; i++)  {
        value <<= 1;
        value += inputBit();
    }
}

int bwtzip::arithmetic::ArithDecoder::decodeSymbol() {
    int sym;

    unsigned long int range = high - low + 1;
    unsigned long int cum = ((value - low + 1) * acm[0] - 1) / range;

    for (sym = 0; acm[sym + 1] > cum; sym++);

    high = low + (range * acm[sym]) / acm[0] - 1;
    low  = low + (range * acm[sym + 1]) / acm[0];

    for (;;)  {
        if (high < HALF)  {

        } else if (low >= HALF)  {
            value -= HALF;
            low -= HALF;
            high -= HALF;
        } else if (low >= FIRST_QTR && high < THIRD_QTR) {
            value -= FIRST_QTR;
            low -= FIRST_QTR;
            high -= FIRST_QTR;
        } else {
            break;
        }
        low = 2 * low;
        high = 2 * high + 1;
        value = 2 * value + inputBit();
    }

    acm.update(sym);

    return sym;
}

bool bwtzip::arithmetic::ArithDecoder::inputBit() {
    if (loc < pvec->size() * 8) {
        bool ret = ((*pvec)[loc / 8] >> (7 - loc % 8)) & 1;
        ++loc;
        return ret;
    } else {
        return 0;
    }
}

vector<unsigned char>& bwtzip::arith(vector<unsigned char>& v) {
    ArithEncoder ae;

    for (size_t i = 0; i < v.size(); i++)  {
        ae.encodeSymbol(v[i]);
    }
    ae.encodeSymbol(SENTINEL);

    v = ae.finalize();
    return v;
}

vector<unsigned char>& bwtzip::unarith(vector<unsigned char>& v) {
    vector<unsigned char> decompressed;
    int decoded;

    ArithDecoder ad(&v);

    while ((decoded = ad.decodeSymbol()) != SENTINEL) {
        decompressed.push_back(decoded);
    }

    swap(v, decompressed);
    return v;
}
