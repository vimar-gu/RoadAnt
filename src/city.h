#ifndef CITY_H
#define CITY_H
#include <iostream>
#include <QObject>
#include <vector>
#include "singleton.hpp"
#include "ant.h"
using namespace std;

struct CPos {
    CPos() {}
    CPos(int x, int y) :_x(x), _y(y) {}
    int _x;
    int _y;
};

class CRoad {
public:
    CRoad() {}
    CRoad(CPos start, CPos end, int level) : _start(start), _end(end), _level(level) {
        if (_start._x == _end._x) _length = abs(_start._y - _end._y);
        else _length = abs(_start._x - _end._x);
    }
    const CPos start() const {return _start;}
    const CPos end() const {return _end;}
    const int& level() {return _level;}
    const int& length() {return _length;}
private:
    CPos _start;
    CPos _end;
    int _level;
    int _length;
};

class CTarget : public CRoad {
public:
    CTarget() {}
    CTarget(CRoad road, int dist) : CRoad(road), _dist(dist) {}
    const int& dist() const {return _dist;}
private:
    int _dist;
};
typedef CTarget CStore;

class CPack {
public:
    CPack() {}
    CPack(CTarget source, CTarget destination) : _source(source), _destination(destination), _state(0) {}
    const CTarget source() {return _source;}
    const CTarget destination() {return _destination;}
    const int& reward() {return _reward;}
    const int& state() {return _state;}
    void setState() {_state = 1;}
private:
    CTarget _source;
    CTarget _destination;
    int _reward;
    int _state;
};

class CDriver : public CRoad {
public:
    CDriver() {}
    CDriver(CRoad road, double dist) : CRoad(road), _dist(dist) {}
    int dist2Target(CTarget c);
    const int& dist() {return _dist;}
    void catchPack(CPack& p);
private:
    int _dist; // from the start end
    vector<CPack> _packHolding;
};

class CCity : public QObject {
    Q_OBJECT
public:
    CCity();
    void start();
    void generatePack();
signals:
    void needDraw();
public slots:
    void fresh();
private:
    vector<CRoad> _roadList;
    vector<CStore> _storeList;
    vector<CDriver> _driverList;
    vector<CPack> _packWaiting;
    int _roadNum;
    int _storeNum;
    int _driverNum;
};

typedef Singleton<CCity> City;
#endif // CITY_H
