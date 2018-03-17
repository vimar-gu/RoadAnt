#ifndef ANT_H
#define ANT_H
#include <iostream>
#include <vector>
#include "city.h"
using namespace std;

class ant
{
public:
    ant();
    ~ant();
private:
    int _maxLoop;
    int _antNum;
    int _packNum;
    int _driverNum;
    double _enFactor = 0.5;
    double _deFactor = 0.5;
};

#endif // ANT_H
