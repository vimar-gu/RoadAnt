#include "utils.h"
#include <iostream>
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
    if (d.start()._x == d.end()._x) { // the driver's road is horizontal
        if (t.start()._x == t.end()._x) {
            return t.length() - t.dist() + d.length() - d.dist() < t.dist() + d.dist();
        }
        else return t.pos()._x > d.pos()._x;
    }
    else {
        if (t.start()._y == t.end()._y) {
            return t.length() - t.dist() + d.length() - d.dist() < t.dist() + d.dist();
        }
        else return t.pos()._y > d.pos()._y;
    }
}

int distBetween(CTarget d, CTarget c) {
    int result;
    CPos dStart = d.start();
    CPos dEnd = d.end();
    CPos cStart = c.start();
    CPos cEnd = c.end();

    if (dStart._x == dEnd._x) { // the driver's road is horizontal
        if (cStart._x == cEnd._x) { // the store's road is horizontal
            if (cStart._y == dStart._y) { // on the same column
                result = abs(cStart._x - dStart._x) + min(d.dist() + c.dist(), 2 * (cEnd._y - cStart._y) - d.dist() - c.dist());
            }
            else {
                result = abs(cStart._x - dStart._x) + abs(cStart._y + c.dist() - dStart._y - d.dist());
            }
        }
        else { // the store's road is vertical
            result = abs(cStart._x + c.dist() - dStart._x) + abs(cStart._y - dStart._y - d.dist());
        }
    }
    else { // the driver's road is vertical
        if (cStart._y == cEnd._y) { //the store's road is vertical
            if (cStart._x == dStart._x) { // on the same row
                result = abs(cStart._y - dStart._y) + min(d.dist() + c.dist(), 2 * (cEnd._x - cStart._x) - d.dist() - c.dist());
            }
            else {
                result = abs(cStart._y - dStart._y) + abs(cStart._x + c.dist() - dStart._x - d.dist());
            }
        }
        else {
            result = abs(cStart._y + c.dist() - dStart._y) + abs(cStart._x - dStart._x - d.dist());
        }
    }
    return result;
}

int distBetween(CDriver d, CTarget c) {
    CRoad tmpRoad(d.start(), d.end(), d.level());
    CTarget tmpTarget(tmpRoad, d.dist());
    return distBetween(tmpTarget, c);
}

double calcCost(vector<CTarget> targetList) {
    return 0;
}
