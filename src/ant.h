#ifndef ANT_H
#define ANT_H
#include <iostream>
#include <vector>
#include "city.h"
using namespace std;

class Ant
{
public:
    Ant(int antNum, int loopNum) : _maxLoop(loopNum), _antNum(antNum) {}
    ~Ant() {}
    void dealwithData(CDriver& d, vector<CPack> packList);
    void randList(vector<vector<int>> list);
private:
    int _maxLoop;
    int _antNum;
    int _driverNum;
    int _routeNum;
    int _targetNum;
    double _enFactor = 0.5;
    double _deFactor = 0.5;
    vector<double> _routeTau;
    vector<int> _bestRouteRec;
};

#endif // ANT_H
