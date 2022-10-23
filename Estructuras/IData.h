#ifndef IDATA

#define IDATA 1

#include <string>

using namespace std;

class IData{
    public:
        virtual int compareTo(IData *pToCompare) = 0;

        virtual string toString() = 0;
};

#endif