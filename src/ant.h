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
    bool isOrigin() {return _isOrigin;}
    CTarget dealwithData(CDriver& d, vector<CPack> packList);
    vector<CTarget> dealwithHolding(CDriver& d, vector<CPack> driverList);
    void dealwithWaiting(CDriver& d, vector<CTarget>& targetList, vector<CPack> packList);
private:
    bool _isOrigin = 0;
    int _maxLoop;
    int _antNum;
    int _driverNum;
    int _routeNum;
    int _targetNum;
    int _totalTime;
    double _enFactor = 0.5;
    double _deFactor = 0.5;
    vector<double> _routeTau;
    vector<int> _bestRouteRec;
};

#endif // ANT_H
