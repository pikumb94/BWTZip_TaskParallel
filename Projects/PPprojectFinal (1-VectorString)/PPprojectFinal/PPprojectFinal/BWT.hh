// bwtzip_suffixtree.hh - bwtzip Suffix Tree functions
// GPLed - Stephan T. Lavavej - http://stl.caltech.edu

#ifndef BWT_HH
#define BWT_HH

#include <vector>
#include <utility>
using namespace std;

string BWT(string in);

namespace bwtzip {
    std::vector<unsigned char>& bwtStree(std::vector<unsigned char>& v);

    namespace ukkonen {
        class SuffixNode;

        struct Transition {
            short        letter;
            long         left;
            long         right;
            SuffixNode * child;

            Transition(short letter, long left, long right, SuffixNode *child)
                : letter(letter), left(left), right(right), child(child) { }
            bool operator<(const Transition& rhs) const { return letter < rhs.letter; }
            bool operator>(const Transition& rhs) const { return letter > rhs.letter; }
            bool operator==(short a)              const { return letter == a; }
        };

        class TransitionList {
        public:
            TransitionList();
            bool        hasChildren() const;
            Transition  getFirstChild() const;
            void        removeFirstChild();
            bool        doesTransitionExist(short a) const;
            Transition  findTransition(short a) const;
            void        insertTransition(const Transition& t);

        private:
            std::vector<Transition> vec;
        };

        class SuffixNode {
        public:
            TransitionList children;
            SuffixNode *   link;

            SuffixNode();
            ~SuffixNode();
            bool haschildren() const;
            static void dfs(std::vector<size_t>& v, SuffixNode * root);

            static void displayInstances();

        private:
            static size_t count;
        };

        class SuffixTree {
        public:
            SuffixTree(const std::vector<unsigned char>& v);
            std::vector<unsigned char> getTransformed();
            ~SuffixTree();

        private:
            SuffixNode *          root;
            SuffixNode            bottom;
            short *               sanitizedtext;
            const size_t          vsize;
            const size_t          sentineledlength;
            const long            infinity;  

            std::pair<SuffixNode *, long> canonize(SuffixNode * s, long k, long p);
            std::pair<bool, SuffixNode *> test_and_split(SuffixNode * s, long k, long p, short t);

            std::pair<SuffixNode *, long> update(std::pair<SuffixNode *, long> mypair, long i);

            static short * initText(const std::vector<unsigned char>& v);
            std::vector<size_t> lengthsOfSortedSuffixes();
        };
    }
}

#endif
