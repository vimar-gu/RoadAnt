#include "utils.h"
#include <iostream>
#include <QDebug>
using namespace std;

bool operator ==(const CPos p1, const CPos p2) {
    return p1._x == p2._x && p1._y == p2._y;
}

bool operator ==(const CTarget s1, const CTarget s2) {
    return s1.start() == s2.start() && s1.end() == s2.end() && s1.dist() == s2.dist();
}
bool operator ==(const CPack& p1, const CPack p2) {
    return p1.source() == p2.source() && p1.destination() == p2.destination() && p1.reward() == p2.reward();
}

bool isOntheRight(const CDriver &d, const CTarget &t) { // whether target is on the right of driver
    if (d.vertical()) {
        if (d.sameRoad(t)) return t.pos()._y > d.pos()._y;
        else if (d.dist() == 0 || d.dist() == d.length()) return t.pos()._x > d.pos()._x;
        else if (d.parallel(t)) return t.length() - t.dist() + d.length() - d.dist() < t.dist() + d.dist();
        else return t.pos()._y > d.pos()._y;
    }
    else {
        if (d.sameRoad(t)) return t.pos()._x > d.pos()._x;
        else if (d.dist() == 0 || d.dist() == d.length()) return t.pos()._y > d.pos()._y;
        else if (d.parallel(t)) return t.length() - t.dist() + d.length() - d.dist() < t.dist() + d.dist();
        else return t.pos()._x > d.pos()._x;
    }
}

int distBetween(CTarget d, CTarget c) {
    int result;
    CPos dStart = d.start();
    CPos dEnd = d.end();
    CPos cStart = c.start();
    CPos cEnd = c.end();

    if (dStart._x == dEnd._x) { // the driver's road is vertical
        if (cStart._x == cEnd._x && cStart._x != dStart._x && cStart._y == dStart._y) { // the store's road is vertical
                result = abs(c.pos()._x - d.pos()._x) + min(abs(c.length() - c.dist() + d.length() - d.dist()), c.dist() + d.dist());
        }
        else { // the store's road is horizontal
            result = abs(d.pos()._x - c.pos()._x) + abs(d.pos()._y - c.pos()._y);
        }
    }
    else { // the driver's road is horizontal
        if (cStart._y == cEnd._y && cStart._y != dStart._y && cStart._x == dStart._x) { //the store's road is horizontal
                result = abs(c.pos()._y - d.pos()._y) + min(abs(c.length() - c.dist() + d.length() - d.dist()), c.dist() + d.dist());
        }
        else {
            result = abs(d.pos()._x - c.pos()._x) + abs(d.pos()._y - c.pos()._y);
        }
    }
    return result;
}

int distBetween(CDriver d, CTarget c) {
    CRoad tmpRoad(d.start(), d.end(), d.level());
    CTarget tmpTarget(tmpRoad, d.dist());
    return distBetween(tmpTarget, c);
}

int distBetween(CDriver d, CPos p) {
    CPos dPos = d.pos();
    return abs(dPos._x - p._x) + abs(dPos._y - p._y);
}

int getMinDist(CDriver d, CTarget t) {
    int minDist = 10000;
    if (distBetween(d, t) > 0) minDist = distBetween(d, t);
    if (distBetween(d, d.start()) > 0 && distBetween(d, d.start()) < minDist) minDist = distBetween(d, d.start());
    if (distBetween(d, d.end()) > 0 && distBetween(d, d.end()) < minDist) minDist = distBetween(d, d.end());
    return minDist;
}

double calcCost(vector<CTarget> targetList) {
    double cost = 0;

    // add distance cost
    CTarget thisTarget, nextTarget;
    for (unsigned int i = 0; i < targetList.size() - 1; i++) {
        thisTarget = targetList.at(i);
        nextTarget = targetList.at(i + 1);
        cost += distBetween(thisTarget, nextTarget);
    }

    return cost;
}

int calcTime(CPack p, CTarget t) {
    CTarget tmps;
    tmps.setStart(p.source().start());
    tmps.setEnd(p.source().end());
    tmps.setDist(p.source().dist());
    CTarget tmpd;
    tmpd.setStart(p.source().start());
    tmpd.setEnd(p.source().end());
    tmpd.setDist(p.source().dist());

    return distBetween(t, tmps) + distBetween(tmps, tmpd);
}
