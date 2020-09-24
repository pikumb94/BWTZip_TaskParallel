// bwtzip_suffixtree.cc - bwtzip Suffix Tree functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#include "pch.h"

#include "common.hh"
#include "BWT.hh"
#include <stack>
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace bwtzip;
using namespace bwtzip::ukkonen;

string BWT(string in) {
	string out;

	vector <unsigned char> v;
	v.assign(in.begin(), in.end());
	bwtzip::bwtStree(v);
	out.assign(v.begin(), v.end());

	return  out;
}

size_t bwtzip::ukkonen::SuffixNode::count = 0;

bwtzip::ukkonen::TransitionList::TransitionList() {
    vec.reserve(2);
}

bool bwtzip::ukkonen::TransitionList::hasChildren() const {
    return !vec.empty();
}

Transition bwtzip::ukkonen::TransitionList::getFirstChild() const {
    return vec.back();
}

void bwtzip::ukkonen::TransitionList::removeFirstChild() {
    vec.pop_back();
}

bool bwtzip::ukkonen::TransitionList::doesTransitionExist(short a) const {
    for (vector<Transition>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        if (*it == a) {
            return true;
        }
    }

    return false;
}

Transition bwtzip::ukkonen::TransitionList::findTransition(short a) const {
    for (vector<Transition>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        if (*it == a) {
            return *it;
        }
    }

    cout << "Problem in findTransition()!" << endl;
    exit(EXIT_FAILURE);
}

void bwtzip::ukkonen::TransitionList::insertTransition(const Transition& t) {
    for (vector<Transition>::iterator it = vec.begin(); it != vec.end(); ++it) {
        if (t < *it) {         
        } else if (t > *it) {   
            vec.insert(it, t);  
            return;
        } else {               
            *it = t;            
            return;
        }
    }

    vec.push_back(t); 
}


bwtzip::ukkonen::SuffixNode::SuffixNode()
    : link(NULL) {

    count++;
}

bwtzip::ukkonen::SuffixNode::~SuffixNode() {
    count--;
}

bool bwtzip::ukkonen::SuffixNode::haschildren() const {
    return children.hasChildren();
}


void bwtzip::ukkonen::SuffixNode::dfs(vector<size_t>& v, SuffixNode * root) {
    stack<pair<SuffixNode *, size_t> > stak;
    stak.push(make_pair(root, (size_t) 0));

    while (!stak.empty()) {
        if (stak.top().first->haschildren()) {
            Transition temp = stak.top().first->children.getFirstChild();
            stak.top().first->children.removeFirstChild();
            if (temp.child != NULL) {
                pair<SuffixNode *, size_t> mypair(
                    temp.child,
                    stak.top().second + temp.right - temp.left + 1
                );
                stak.push(mypair);
            } else { 
                v.push_back(stak.top().second + temp.right - temp.left + 1); 
            }
        } else {    
            delete stak.top().first;    
            stak.pop();
        }
    }
}

void bwtzip::ukkonen::SuffixNode::displayInstances() {
}


bwtzip::ukkonen::SuffixTree::SuffixTree(const vector<unsigned char>& v)
    : root(new SuffixNode),
      sanitizedtext(initText(v)),
      vsize(v.size()),
      sentineledlength(v.size() + 1),
      infinity(v.size()) {

    for (int j = 1; j <= 257; j++) {
        bottom.children.insertTransition(Transition(sanitizedtext[-j], -j, -j, root));
    }

    root->link = &bottom;

    pair<SuffixNode *, long> temp(root, 0);

    for (long i = 0; i < (long) v.size() + 1; i++) {
        temp = update(temp, i);
        temp = canonize(temp.first, temp.second, i);
    }
}

vector<size_t> bwtzip::ukkonen::SuffixTree::lengthsOfSortedSuffixes() {
    vector<size_t> ret;
    ret.reserve(sentineledlength);
    SuffixNode::dfs(ret, root);
    return ret;
}

vector<unsigned char> bwtzip::ukkonen::SuffixTree::getTransformed() {
    vector<unsigned char> transformed(sentineledlength);
    const vector<size_t> l = lengthsOfSortedSuffixes();

    vector<unsigned char> primaryindex;
    vector<unsigned char> sentinelindex;

    for (size_t i = 0; i < l.size(); i++) {
        if (l[i] < vsize) {
            transformed[i] = sanitizedtext[vsize - l[i]];
        } else if (l[i] == vsize) {
            primaryindex = encodeUL(i);
            transformed[i] = sanitizedtext[0];
        } else {                       
            sentinelindex = encodeUL(i);
            transformed[i] = 0;         
        }
    }

    return primaryindex + sentinelindex + transformed;
}

bwtzip::ukkonen::SuffixTree::~SuffixTree() {
    sanitizedtext -= 257;
    delete[] sanitizedtext;
}

pair<SuffixNode *, long> bwtzip::ukkonen::SuffixTree::canonize(SuffixNode * s, long k, long p) {
    if (p >= k) {
        Transition temp = s->children.findTransition(sanitizedtext[k]);

        while (temp.right - temp.left <= p - k) {
            k = k + temp.right - temp.left + 1;
            s = temp.child;
            if (k <= p) {
                temp = s->children.findTransition(sanitizedtext[k]);
            }
        }
    }

    return make_pair(s, k);
}

pair<bool, SuffixNode *> bwtzip::ukkonen::SuffixTree::test_and_split(SuffixNode * s, long k, long p, short t) {
    if (k <= p) {
        Transition temp = s->children.findTransition(sanitizedtext[k]);
        long kprime = temp.left;
        long pprime = temp.right;
        SuffixNode * sprime = temp.child;

        if (t == sanitizedtext[kprime + p - k + 1]) {
            return make_pair(true, s);
        } else {
            SuffixNode * r = new SuffixNode;

            s->children.insertTransition(
                Transition(sanitizedtext[kprime], kprime, kprime + p - k, r));
            r->children.insertTransition(
                Transition(sanitizedtext[kprime + p - k + 1], kprime + p - k + 1, pprime, sprime));

            return make_pair(false, r);
        }
    }

    return make_pair(s->children.doesTransitionExist(t), s);
}

pair<SuffixNode *, long> bwtzip::ukkonen::SuffixTree::update(pair<SuffixNode *, long> mypair, long i) {
    SuffixNode * oldr = root;
    pair<bool, SuffixNode *> temp = test_and_split(mypair.first, mypair.second, i - 1, sanitizedtext[i]);
    bool endpoint = temp.first;
    SuffixNode * r = temp.second;
    while (!endpoint) {
        r->children.insertTransition(Transition(sanitizedtext[i], i, infinity, NULL));

        if (oldr != root) {
            oldr->link = r;
        }
        oldr = r;
        mypair = canonize(mypair.first->link, mypair.second, i - 1);
        temp = test_and_split(mypair.first, mypair.second, i - 1, sanitizedtext[i]);
        endpoint = temp.first;
        r = temp.second;
    }
    if (oldr != root) {
        oldr->link = mypair.first;
    }
    return mypair;
}

short * bwtzip::ukkonen::SuffixTree::initText(const vector<unsigned char>& v) {
    short * ret = new short[v.size() + 258];

    for (size_t i = 0; i < 257; i++) {
        ret[i] = 256 - i;
    }

    ret += 257;

    for (size_t i = 0; i < v.size(); i++) {
        ret[i] = v[i];
    }

    ret[v.size()] = 256;  

    return ret;
}


vector<unsigned char>& bwtzip::bwtStree(vector<unsigned char>& v) {
    if (v.size() < 2) {
        cout << "ERROR: vector of length less than 2 passed to bwtzip::bwtStree()." << endl;
        exit(EXIT_FAILURE);
    }

    SuffixNode::displayInstances();
    {
        SuffixTree suffixtree(v);
        SuffixNode::displayInstances();
        v = suffixtree.getTransformed();
    }
    SuffixNode::displayInstances();

    return v;
}
