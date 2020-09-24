// bwtzip_mtf.hh - bwtzip Move-To-Front functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

// The MTF-1 algorithm changes how things are moved in the list. If the
// byte was at any position > 1, then it is moved to position 1. If the
// byte was at position 1, then it is moved to position 0. Otherwise it
// is not moved.

// The MTF-2 algorithm further changes how things are moved in the list.
// If the byte was at any position > 1, then it is moved to position 1.
// If the byte was at position 1 and the last output byte was not 0,
// then it is moved to position 0. Otherwise it is not moved.

#ifndef MTF_HH
#define MTF_HH

#include <vector>

using namespace std;

string MTF(string in);

namespace bwtzip {
    std::vector<unsigned char>& mtf(std::vector<unsigned char>& v);
    std::vector<unsigned char>& mtf1(std::vector<unsigned char>& v);
    std::vector<unsigned char>& mtf2(std::vector<unsigned char>& v);
    std::vector<unsigned char>& ts0(std::vector<unsigned char>& v);

    std::vector<unsigned char>& unmtf(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unmtf1(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unmtf2(std::vector<unsigned char>& v);
    std::vector<unsigned char>& unts0(std::vector<unsigned char>& v);
}
template <typename T1, typename T2, typename T3> struct trio {
	T1 first;
	T2 second;
	T3 third;

	trio() : first(), second(), third() { }

	trio(const T1& a, const T2& b, const T3& c)
		: first(a), second(b), third(c) { }

	template <typename U1, typename U2, typename U3>
	trio(const trio<U1, U2, U3>& t)
		: first(t.first), second(t.second), third(t.third) { }
};
#endif
