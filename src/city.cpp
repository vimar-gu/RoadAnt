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

    CRoad tmpRoad = _roadList.front();
    CDriver driver(tmpRoad, 96);
    CDriver driver2(tmpRoad, 10);
    _driverList.push_back(driver);
    _driverList.push_back(driver2);
    _driverNum = 2;
    _roadNum = _roadList.size();
    _storeNum = _storeList.size();
    roadData.close();
    storeData.close();
}

void CCity::start() {
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(fresh()));
    timer->start(50);
}

void CCity::generatePack() {
    int r = rand() % (_storeNum - 1);
    CStore s = _storeList.at(r);

    auto pos = find_if(_packWaiting.begin(), _packWaiting.end(), [=](CPack p) {return p.source() == s;});
    if (pos != _packWaiting.end()) return;

    for (CDriver& elem : _driverList) {
        //if (elem.start() == s.start() || elem.start() == s.end() || elem.end() == s.start() || elem.end() == s.end()) return;
        for (CPack& packElem : elem.pickingPacks()) {
            if (packElem.source() == s) return;
        }
    }

    r = rand() % (_roadNum - 1);
    CRoad road = _roadList.at(r);
    int d = rand() % road.length();
    CTarget t(road, d);
    CPack p(s, t);
    _packWaiting.push_back(p);
}

void CCity::calcVel(CDriver& d, vector<CPack> packList) {
    Ant ant(10, 1);
    vector<CTarget> targetList = ant.dealwithData(d, packList);
    CTarget nextTarget;
    CVel vel;
    if (targetList.empty()) vel = CVel(0, 0);
    else {
        nextTarget = targetList.front();
        qDebug() << d.pos()._x << d.pos()._y << nextTarget.pos()._x << nextTarget.pos()._y;
        bool dir = isOntheRight(d, nextTarget);
        int level = d.level();
        if (d.vertical()) { // the driver's road is vertical
            if (!d.sameRoad(nextTarget) && (d.dist() == 0 || d.dist() == d.length()))
                vel = dir ? CVel(level, 0) : CVel(-level, 0);
            else
                vel = dir ? CVel(0, level) : CVel(0, -level);
        }
        else {
            if (!d.sameRoad(nextTarget) && (d.dist() == 0 || d.dist() == d.length()))
                vel = dir ? CVel(0, level) : CVel(0, -level);
            else
                vel = dir ? CVel(level, 0) : CVel(-level, 0);
        }

        //qDebug() << d.pos()._x << d.pos()._y << nextTarget.pos()._x << nextTarget.pos()._y << vel._x << vel._y << isOntheRight(d, nextTarget);
        //set picking packs
        for (auto& elem : targetList) {
            auto pos = find_if(_packWaiting.begin(), _packWaiting.end(), [=](CPack p){return p.source() == elem;});
            if (pos != _packWaiting.end()) {
                qDebug() << "pick";
                d.pickPack(*pos);
                _packWaiting.erase(pos);
            }
        }
    }
    d.setVel(vel);
}

void CCity::applyVel(CDriver& d) {
    if (d.pos() == d.start() || d.pos() == d.end()) {
        qDebug() << "change";
        if (d.vel()._x > 0) {
            auto r = find_if(_roadList.begin(), _roadList.end(), [=](CRoad r)
            {return r.start() == d.pos() && r.end()._x > r.start()._x;});
            d.changeRoad(*r);
        }
        else if (d.vel()._x < 0) {
            auto r = find_if(_roadList.begin(), _roadList.end(), [=](CRoad r)
            {return r.end() == d.pos() && r.end()._x > r.start()._x;});
            d.changeRoad(*r);
        }
        else if (d.vel()._y > 0) {
            auto r = find_if(_roadList.begin(), _roadList.end(), [=](CRoad r)
            {return r.start() == d.pos() && r.end()._y > r.start()._y;});
            d.changeRoad(*r);
        }
        else if (d.vel()._y < 0) {
            auto r = find_if(_roadList.begin(), _roadList.end(), [=](CRoad r)
            {return r.end() == d.pos() && r.end()._y > r.start()._y;});
            d.changeRoad(*r);
        }
    }
    else {
        d.setDist(d.dist() + d.vel().mod());
    }
    if (d.isPicking()) {
        if(d.checkHold()) {
            _pickedCase++;
        }
    }
    if (d.isHolding()) {
        if(d.checkDrop()) {
            _finishedCase++;
        }
    }
}

vector<CPack> CCity::setPackList(CDriver &d) {
    vector<CPack> packList;
    int density = calcDensity(d);
    CPos start = d.start();
    CPos end = d.end();
    if (density <= smallRangeDensity) {

    }
    else if (density <= midRangeDensity) {

    }
    else { // need to change working range

    }
    for (CPack elem : _packWaiting) {
        if ((d.isPicking() || d.isHolding()) && (elem.source().start() == start || elem.source().end() == start
                                                 || elem.source().start() == start || elem.source().start() == end)) continue;
        else packList.push_back(elem);
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

    generatePack();
    for (int i = 0; i < _driverNum; i++) {
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

void CDriver::pickPack(CPack p) {
    p.setStatePick();
    _packPicking.push_back(p);
}

void CDriver::catchPack(CPack p) {
    p.setStateHold();
    _packHolding.push_back(p);
}

bool CDriver::checkHold() {
    for (auto& elem : _packPicking) {
        if (pos() == elem.source().pos()) {
            qDebug() << "hold";
            CPack holding = elem;
            holding.setStateHold();
            _packHolding.push_back(holding);
            auto elempos = find_if(_packPicking.begin(), _packPicking.end(), [=](CPack p){return p.source().pos() == pos();});
            _packPicking.erase(elempos);
            return true;
        }
    }
    return false;
}

bool CDriver::checkDrop() {
    auto elempos = find_if(_packHolding.begin(), _packHolding.end(), [=](CPack p){return p.destination().pos() == pos();});
    if (elempos != _packHolding.end()) {
        qDebug() << "drop";
        _packHolding.erase(elempos);
        return true;
    }
    return false;
}

void CDriver::changeRoad(CRoad road) {
     setStart(road.start());
     setEnd(road.end());
     setLevel(road.level());
     if (_vel.mod() > 0) setDist(1);
     else setDist(road.length() - 1);
}

bool CRoad::parallel(CRoad r) const {
    if (this->vertical() && r.vertical()) return this->start()._x != r.start()._x;
    else if (!this->vertical() && !r.vertical()) return this->start()._y != r.start()._y;
    else return false;
}

bool CRoad::sameRoad(CRoad r) const {
    if (this->vertical() && r.vertical()) return this->start()._x == r.start()._x;
    else if (!this->vertical() && !r.vertical()) return this->start()._y == r.start()._y;
    else return false;
}
