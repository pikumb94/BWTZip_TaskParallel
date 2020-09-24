// bwtzip_common.hh - bwtzip Common functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef COMMON_HH
#define COMMON_HH

#include <vector>

namespace bwtzip {
    const unsigned long long BWTZIP_SIG             = 0x894257540D0A1A0AULL;
    const unsigned long long MIN_LENGTH_TO_COMPRESS = 300;
    const short              SENTINEL               = 256;
    const unsigned long      MAX_CHUNK_SIZE         = 5 * 1048576;

    template <typename T>
    const std::vector<T> operator+(const std::vector<T>& lhs, const std::vector<T>& rhs) {

        std::vector<T> ret = lhs;
        ret += rhs;
        return ret;
    }

    template <typename T>
    std::vector<T>& operator+=(std::vector<T>& lhs, const std::vector<T>& rhs) {

        lhs.insert(lhs.end(), rhs.begin(), rhs.end());
        return lhs;
    }

    // Return the nth big-endian byte of an unsigned long. 0 <= n <= 3
    unsigned char decomposeUL(unsigned long ul, int n);

    // Return the nth big-endian byte of an unsigned long long. 0 <= n <= 7
    unsigned char decomposeULL(unsigned long long ull, int n);

    unsigned long long bitsToBytes(unsigned long long bits);

    // Return the unsigned long encoded big-endian by
    // the 4 bytes starting at location n.
    unsigned long readUL(const std::vector<unsigned char>& v, size_t n);

    // Return the unsigned long long encoded big-endian by
    // the 8 bytes starting at location n.
    unsigned long long readULL(const std::vector<unsigned char>& v, size_t n);

    // Return a vector<unsigned char> that represents ul big-endian.
    std::vector<unsigned char> encodeUL(unsigned long ul);

    // Return a vector<unsigned char> that represents ull big-endian.
    std::vector<unsigned char> encodeULL(unsigned long long ull);

    class PackedBits {
    public:
        PackedBits();
        void push_back(bool bit);
        std::vector<unsigned char> get_vuc();

    private:
        std::vector<unsigned char> data;
        unsigned char              waiting;
        int                        numwaiting;
    };
}

#endif // End of: #ifndef BWTZIP_COMMON_HH
