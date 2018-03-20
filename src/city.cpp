#include <QDebug>
#include "city.h"
#include "utils.h"
#include "ant.h"
#include <math.h>
#include <sys/time.h>
#include <iostream>
#include <QTimer>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <algorithm>
using namespace std;
QString ROAD_DATA_DIR = "/home/ypbehere/Documents/srtp/RoadAnt/RoadAnt/data/roadData.txt";
QString STORE_DATA_DIR = "/home/ypbehere/Documents/srtp/RoadAnt/RoadAnt/data/storeData.txt";

namespace {
    double smallRangeDensity = 2;
    double midRangeDensity = 5;
}

CCity::CCity()
{
    QFile roadData(ROAD_DATA_DIR);
    QFile storeData(STORE_DATA_DIR);
    if (!roadData.open(QIODevice::ReadOnly | QIODevice::Text) || !storeData.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "Open Failed" << endl;
    }

    QTextStream roadDataStream(&roadData);
    int x, y, level;
    while (!roadDataStream.atEnd()) {
        roadDataStream >> x >> y;
        CPos start(x, y);
        roadDataStream >> x >> y;
        CPos end(x, y);
        roadDataStream >> level;
        CRoad r(start, end, level);
        _roadList.push_back(r);
    }

    QTextStream storeDataStream(&storeData);
    int dist;
    while (!storeDataStream.atEnd()) {
        storeDataStream >> x >> y;
        CPos start(x, y);
        storeDataStream >> x >> y;
        CPos end(x, y);
        auto tmpRoad = find_if(_roadList.begin(), _roadList.end(), [=](CRoad r){return r.start() == start && r.end() == end;});
        storeDataStream >> dist;
        CStore s(*tmpRoad, dist);
        _storeList.push_back(s);
    }

    _roadNum = _roadList.size();
    _storeNum = _storeList.size();
    roadData.close();
    storeData.close();
}

void CCity::start() {
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(fresh()));
    timer->start(1000);
}

void CCity::generatePack() {
    int r = rand() % _storeNum;
    CStore s = _storeList.at(r);
    auto pos = find_if(_packWaiting.begin(), _packWaiting.end(), [=](CPack p) {return p.source() == s;});
    while (pos == _packWaiting.end()) {
        r = rand() % _storeNum;
        s = _storeList.at(r);
        pos = find_if(_packWaiting.begin(), _packWaiting.end(), [=](CPack p) {return p.source() == s;});
    }
    CRoad road = _roadList.at(r);
    int d = rand() % road.length();
    CTarget t(road, d);
    CPack p(s, t);
    _packWaiting.push_back(p);
}

void CCity::calcVel(CDriver& d, vector<CPack> packList) {
    Ant ant(10, 1);
    ant.dealwithData(d, packList);
}

void CCity::applyVel(CDriver& d) {
    if (d.pos() == d.start() || d.pos() == d.end()) {
        if (d.vel()._x > 0) {
            auto r = find_if(_roadList.begin(), _roadList.end(), [=](CRoad r)
            {return r.start() == d.pos() && r.end()._x > r.start()._x;});
            d.changeRoad(*r);
        }
    }
    else {
        d.setDist(d.dist() + d.vel().mod());
    }
}

vector<CPack> CCity::setPackList(CDriver &d) {
    vector<CPack> packList;
    int density = calcDensity(d);
    if (density <= smallRangeDensity) {

    }
    else if (density <= midRangeDensity) {

    }
    else { // need to change working range

    }
    return packList;
}

int CCity::calcDensity(CDriver &d) {
    CPos start = d.start();
    CPos end = d.end();
    int density;

    for (auto& elem : _driverList) {
        if (elem.start() == start || elem.end() == start || elem.start() == end || elem.end() == end) density++;
    }

    return density;
}

void CCity::fresh() {
    emit needDraw();

    for (int i = 0; i < _driverNum - 1; i++) {
        auto& d = _driverList.at(i);
        vector<CPack> packs = setPackList(d);
        calcVel(d, packs);
        applyVel(d);
    }
}

CPos CTarget::pos() const {
    CPos pos;
    if (start()._x == end()._x) pos = CPos(start()._x, start()._y + _dist);
    else pos = CPos(start()._x + _dist, start()._y);
    return pos;
}

CPos CDriver::pos() const {
    CPos pos;
    if (start()._x == end()._x) pos = CPos(start()._x, start()._y + _dist);
    else pos = CPos(start()._x + _dist, start()._y);
    return pos;
}

void CDriver::catchPack(CPack& p) {
    p.setState();
    _packHolding.push_back(p);
}

int CDriver::dist2Target(CTarget c) {
    int result;
    CPos dStart = start();
    CPos dEnd = end();
    CPos cStart = c.start();
    CPos cEnd = c.end();

    if (dStart._x == dEnd._x) { // the driver's road is horizontal
        if (cStart._x == cEnd._x) { // the store's road is horizontal
            if (cStart._y == dStart._y) { // on the same column
                result = abs(cStart._x - dStart._x) + min(_dist + c.dist(), 2 * (cEnd._y - cStart._y) - _dist - c.dist());
            }
            else {
                result = abs(cStart._x - dStart._x) + abs(cStart._y + c.dist() - dStart._y - _dist);
            }
        }
        else { // the store's road is vertical
            result = abs(cStart._x + c.dist() - dStart._x) + abs(cStart._y - dStart._y - _dist);
        }
    }
    else { // the driver's road is vertical
        if (cStart._y == cEnd._y) { //the store's road is vertical
            if (cStart._x == dStart._x) { // on the same row
                result = abs(cStart._y - dStart._y) + min(_dist + c.dist(), 2 * (cEnd._x - cStart._x) - _dist - c.dist());
            }
            else {
                result = abs(cStart._y - dStart._y) + abs(cStart._x + c.dist() - dStart._x - _dist);
            }
        }
        else {
            result = abs(cStart._y + c.dist() - dStart._y) + abs(cStart._x - dStart._x - _dist);
        }
    }
    return result;
}
