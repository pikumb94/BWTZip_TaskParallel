// bwtzip_mtf.cc - bwtzip Move-To-Front functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "pch.h"

#include "MTF.hh"
#include <algorithm>
using namespace std;

void MTF(string &in) {
	//string out;

	vector <unsigned char> v;
	v.assign(in.begin(), in.end());
	bwtzip::mtf2(v);
	in.assign(v.begin(), v.end());

	//return  out;
}


namespace {
    vector<unsigned char> newMtfList() {
        vector<unsigned char> ret(256);
        for (size_t i = 0; i < 256; i++) {
            ret[i] = i;
        }
        return ret;
    }

    vector<trio<unsigned char, int, int> > newTs0List() {
        vector<trio<unsigned char, int, int> > ret(256);
        for (size_t i = 0; i < 256; i++) {
            ret[i] = trio<unsigned char, int, int>(i, -1, -1);
        }
        return ret;
    }

    // Move n bytes from location src to the next higher location.
    void shiftDown(vector<unsigned char>& v, size_t n, size_t src) {
        copy_backward(
            v.begin() + src, 
            v.begin() + src + n, 
            v.begin() + src + n + 1
        );
    }
}

vector<unsigned char>& bwtzip::mtf(vector<unsigned char>& v) {
    vector<unsigned char> mtflist = newMtfList();

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char byte = v[i];                // Input byte
        const unsigned char pos =                       // Find position in list
            find(mtflist.begin(), mtflist.end(), byte) 
            - mtflist.begin();                                  
        v[i] = pos;                                     // Write position
        if (pos != 0) {                                 // If byte wasn't first
            shiftDown(mtflist, pos, 0);                 // Move bytes down
            mtflist[0] = byte;                          // Put byte first
        }
    }

    return v;
}

vector<unsigned char>& bwtzip::unmtf(vector<unsigned char>& v) {
    vector<unsigned char> mtflist = newMtfList();

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char pos = v[i];             // Input position
        const unsigned char byte = mtflist[pos];    // Decoded byte
        v[i] = byte;                                // Write byte
        if (pos != 0) {                             // If byte wasn't first
            shiftDown(mtflist, pos, 0);             // Move bytes down
            mtflist[0] = byte;                      // Put byte first
        }
    }

    return v;
}

vector<unsigned char>& bwtzip::mtf1(vector<unsigned char>& v) {
    vector<unsigned char> mtflist = newMtfList();

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char byte = v[i];
        const unsigned char pos =
            find(mtflist.begin(), mtflist.end(), byte) 
            - mtflist.begin();                                  
        v[i] = pos;

        if (pos == 0) {
            // Nothing to update
        } else if (pos == 1) {
            mtflist[1] = mtflist[0];
            mtflist[0] = byte;
        } else {
            shiftDown(mtflist, pos - 1, 1);
            mtflist[1] = byte;
        }
    }

    return v;
}

vector<unsigned char>& bwtzip::unmtf1(vector<unsigned char>& v) {
    vector<unsigned char> mtflist = newMtfList();

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char pos = v[i];
        const unsigned char byte = mtflist[pos];
        v[i] = byte;
        if (pos == 0) {
            // Nothing to update
        } else if (pos == 1) {
            mtflist[1] = mtflist[0];
            mtflist[0] = byte;
        } else {
            shiftDown(mtflist, pos - 1, 1);
            mtflist[1] = byte;
        }
    }

    return v;
}

vector<unsigned char>& bwtzip::mtf2(vector<unsigned char>& v) {
    vector<unsigned char> mtflist = newMtfList();
    unsigned char lastpos = 0;

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char byte = v[i];
        const unsigned char pos =
            find(mtflist.begin(), mtflist.end(), byte) 
            - mtflist.begin();                                  
        v[i] = pos;

        if (pos == 0) {
            // Nothing to update
        } else if (pos == 1 && lastpos != 0) {
            mtflist[1] = mtflist[0];
            mtflist[0] = byte;
        } else if (pos > 1) {
            shiftDown(mtflist, pos - 1, 1);
            mtflist[1] = byte;
        }

        lastpos = pos;
    }

    return v;
}


vector<unsigned char>& bwtzip::unmtf2(vector<unsigned char>& v) {
    vector<unsigned char> mtflist = newMtfList();
    unsigned char lastpos = 0;

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char pos = v[i];
        const unsigned char byte = mtflist[pos];
        v[i] = byte;

        if (pos == 0) {
            // Nothing to update
        } else if (pos == 1 && lastpos != 0) {
            mtflist[1] = mtflist[0];
            mtflist[0] = byte;
        } else if (pos > 1) {
            shiftDown(mtflist, pos - 1, 1);
            mtflist[1] = byte;
        }

        lastpos = pos;
    }

    return v;
}

vector<unsigned char>& bwtzip::ts0(vector<unsigned char>& v) {
    vector<trio<unsigned char, int, int> > tslist = newTs0List();

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char byte = v[i];
        unsigned char pos = 0;
        for (size_t j = 0; j < tslist.size(); j++) {
            if (tslist[j].first == byte) {
                pos = j;
                break;
            }
        }

        v[i] = pos;

        const int now = i;
        const int lastrequest = tslist[pos].second;
        const trio<unsigned char, int, int> toinsert(byte, now, lastrequest);

        tslist.erase(tslist.begin() + pos);

        // Find the first item in tslist that has been requested at most
        // once since lastrequest.
        // That is to say, find the first item in tslist whose .third < lastrequest
        for (size_t j = 0; j < tslist.size(); j++) {
            if (tslist[j].third < lastrequest) {
                tslist.insert(tslist.begin() + j, toinsert);
                break;
            }
        }

        if (tslist.size() != 256) {
            tslist.push_back(toinsert);
        }
    }

    return v;
}

vector<unsigned char>& bwtzip::unts0(vector<unsigned char>& v) {
    vector<trio<unsigned char, int, int> > tslist = newTs0List();

    for (size_t i = 0; i < v.size(); i++) {
        const unsigned char pos = v[i];
        const unsigned char byte = tslist[pos].first;
        v[i] = byte;

        const int now = i;
        const int lastrequest = tslist[pos].second;
        const trio<unsigned char, int, int> toinsert(byte, now, lastrequest);

        tslist.erase(tslist.begin() + pos);

        // Find the first item in tslist that has been requested at most
        // once since lastrequest.
        // That is to say, find the first item in tslist whose .third < lastrequest
        for (size_t j = 0; j < tslist.size(); j++) {
            if (tslist[j].third < lastrequest) {
                tslist.insert(tslist.begin() + j, toinsert);
                break;
            }
        }

        if (tslist.size() != 256) {
            tslist.push_back(toinsert);
        }
    }

    return v;
}
