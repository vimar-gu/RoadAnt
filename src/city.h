#ifndef CITY_H
#define CITY_H
#include <iostream>
#include <QObject>
#include <vector>
#include "singleton.hpp"
using namespace std;

struct CPos {
    CPos() {}
    CPos(int x, int y) : _x(x), _y(y) {}
    bool valid() {return _x != 0 && _y != 0;}
    int _x;
    int _y;
};

struct CVel {
    CVel() {}
    CVel(int x, int y) : _x(x), _y(y) {}
    bool isPositive() {return mod() > 0;}
    int mod() const {return _x + _y;}
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
    const int& level() const {return _level;}
    const int& length() const {return _length;}
    bool onTheRoad(CPos pos) {return pos._x >= _start._x && pos._x <= _end._x && pos._y >= _start._y && pos._y <= _end._y;}
    bool vertical() const {return _start._x == _end._x;}
    bool parallel(CRoad r) const;
    bool sameRoad(CRoad r) const;
    void setStart(CPos start) {_start = start;}
    void setEnd(CPos end) {_end = end;}
    void setLevel(int level) {_level = level;}
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
    CPos pos() const;
private:
    int _dist;
};
typedef CTarget CStore;

class CPack {
public:
    CPack() {CPos start(0, 0); CPos end(0, 0); CRoad r(start, end, 0); _source = CTarget(r, 0); _destination = CTarget(r, 0);}
    CPack(CTarget source, CTarget destination) : _source(source), _destination(destination), _state(0) {}
    const CTarget source() const {return _source;}
    const CTarget destination() const {return _destination;}
    const int& reward() const {return _reward;}
    const int& state() const {return _state;}
    const int& limit() const {return _timeLimit;}
    void setStateWait() {_state = 0;}
    void setStatePick() {_state = 1;}
    void setStateHold() {_state = 2;}
private:
    CTarget _source;
    CTarget _destination;
    int _reward;
    int _timeLimit;
    int _state; // 0 for waiting, 1 for picking, 2 for holding
};

class CDriver : public CRoad {
public:
    CDriver() {}
    CDriver(CRoad road, double dist) : CRoad(road), _dist(dist), _vel(CVel(0, 0)) {}
    const int& dist() const {return _dist;}
    void setDist(int dist) {_dist = dist;}
    CPos pos() const;
    void catchPack(CPack p);
    void pickPack(CPack p);
    bool isPicking() {return !_packPicking.empty();}
    bool isHolding() {return !_packHolding.empty();}
    bool checkHold();
    bool checkDrop();
    vector<CPack> holdingPacks() {return _packHolding;}
    vector<CPack> pickingPacks() {return _packPicking;}
    void setVel(CVel vel) {_vel = vel;}
    const CVel& vel() const {return _vel;}
    void changeRoad(CRoad road);
private:
    int _dist; // from the start end
    CVel _vel;
    vector<CPack> _packHolding;
    vector<CPack> _packPicking;
};

class CCity : public QObject {
    Q_OBJECT
public:
    CCity();
    void start();
    void generatePack();
    void calcVel(CDriver& d, vector<CPack> packList);
    void applyVel(CDriver& d);
    vector<CPack> setPackList(CDriver& d);
    int calcDensity(CDriver& d);
    int roadNum() {return _roadNum;}
    int storeNum() {return _storeNum;}
    int driverNum() {return _driverNum;}
    int packNum() {return _packWaiting.size();}
    int pickedCase() {return _pickedCase;}
    int finishedCase() {return _finishedCase;}
    CRoad road(int i) {return _roadList.at(i);}
    CDriver driver(int i) {return _driverList.at(i);}
    CStore store(int i) {return _storeList.at(i);}
    CPack pack(int i) {return _packWaiting.at(i);}
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
    int _finishedCase;
    int _pickedCase;
};

typedef Singleton<CCity> City;
#endif // CITY_H
