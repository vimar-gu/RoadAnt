#ifndef CITY_H
#define CITY_H
#include <iostream>
#include <map>
#include <QObject>
#include "singleton.hpp"
#include "ant.h"
using namespace std;

struct pos {
    double _x;
    double _y;
    double x() {return _x;}
    double y() {return _y;}
};

struct CRoad {
    CRoad() {}
    CRoad(pos start, pos end, int level) : _start(start), _end(end), _level(level){}
    const pos start() {return _start;}
    const pos end() {return _end;}
    int level() {return _level;}
    pos _start;
    pos _end;
    int _level;
};

struct CStore : public CRoad {
    CStore() {}
    CStore(CRoad road, double dist) : CRoad(road), _dist(dist){}
    int dist() {return _dist;}
    int _dist; // from the start end
    pos _pos;
    double x() {return _pos.x();}
    double y() {return _pos.y();}
};

struct CCustomer : public CRoad {
    CCustomer() {}
    CCustomer(CRoad road, double dist) : CRoad(road), _dist(dist){}
    int dist() {return _dist;}
    int _dist; // from the start end
};

struct CPack {
    CStore _source;
    CCustomer _target;
    int _weight;
    const CStore source() {return _source;}
    const CCustomer target() {return _target;}
    int weight() {return _weight;}
};

struct CDriver : public CRoad {
    CDriver() {}
    CDriver(int roadNum, double dist) : _roadNum(roadNum), _dist(dist) {}
    double roadNum() {return _roadNum;}
    double dist() {return _dist;}
    int _roadNum; // current location
    double _dist; // from the start end
    list<CPack> packList;
    pos _pos;
    int tempPos;
    double x() {return _pos.x();}
    double y() {return _pos.y();}
};

class CCity : public QObject {
    Q_OBJECT
private:
    const static int _MAX_STORE = 100;
    const static int _MAX_DRIVER = 100;
    int _storeNum;
    int _driverNum = 1; //waiting to be changed with regex
    CStore _store[_MAX_STORE];
    CDriver _driver[_MAX_DRIVER];
signals:
    void needDraw();
public slots:
    void fresh();
public:
    CCity();
    double store2StoreDis(int s1, int s2);
    double store2StoreEta(int s1, int s2);
    CStore& store(int n) {return _store[n];}
    CDriver& driver(int n) {return _driver[n];}
    void start();
    void clearStore(int i);
    int storeNum() {return _storeNum;}
    int driverNum() {return _driverNum;}
};

typedef Singleton<CCity> City;
#endif // CITY_H
