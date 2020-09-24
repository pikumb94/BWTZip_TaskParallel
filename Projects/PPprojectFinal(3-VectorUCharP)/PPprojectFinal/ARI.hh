// bwtzip_arith.hh - bwtzip Adaptive Arithmetic Coding functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// Based on the ANSI C Arithmetic Coding Library by Fred Wheeler (wheeler@cipr.rpi.edu),
// which was adapted from the program in "Arithmetic Coding for Data Compression",
// by Ian H. Witten, Radford M. Neal and John G. Cleary, Communications of the ACM,
// June 1987, Vol. 30, No. 6.

#ifndef ARITH_HH
#define ARITH_HH

#include "common.hh"

using namespace std;

void ARI(vector <unsigned char> &in);

namespace bwtzip {
    std::vector<unsigned char>& arith(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unarith(std::vector<unsigned char>& v);

    namespace arithmetic {
        const unsigned long CODE_VALUE_BITS = 16;
        const unsigned long TOP_VALUE       = (1L << CODE_VALUE_BITS) - 1;
        const unsigned long FIRST_QTR       = TOP_VALUE / 4 + 1;
        const unsigned long HALF            = 2 * FIRST_QTR;
        const unsigned long THIRD_QTR       = 3 * FIRST_QTR;
        const unsigned long NUM_SYMBOLS     = 257;
 
        // Fenwick suggested: 8192, 16
        // I suggest:         16383, 32
        // Original:          16383, 1
        const unsigned long MAX_FREQUENCY   = 16383;
        const unsigned long INCREMENT_VALUE = 32;

        class ArithModel {
        public:
            ArithModel();
            const size_t& operator[](size_t i) const;
            void update(int sym);

        private:
            std::vector<size_t> freq;
            std::vector<size_t> cfreq;
        };

        class ArithEncoder {
        public:
            ArithEncoder();
            void encodeSymbol(int sym);
            std::vector<unsigned char> finalize();

        private:
            unsigned long low;
            unsigned long high;
            unsigned long fbits;
            bwtzip :: PackedBits output;	//bwtzip::PackedBits output;
            ArithModel acm;

            void bitPlusFollow(bool bit);
        };

        class ArithDecoder {
        public:
            ArithDecoder(std::vector<unsigned char> *pv);
            int decodeSymbol();

        private:
            std::vector<unsigned char> * pvec;
            size_t                       loc;
            unsigned long                value;
            unsigned long                low;
            unsigned long                high;
            ArithModel                   acm;

            bool inputBit();
        };
    }
}

#endif // End of: #ifndef BWTZIP_ARITH_HH
