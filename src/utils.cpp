#include "utils.h"
#include <iostream>
using namespace std;

bool operator ==(const CPos p1, const CPos p2) {
    return p1._x == p2._x && p1._y == p2._y;
}

bool operator ==(const CStore s1, const CStore s2) {
    return s1.start() == s2.start() && s1.end() == s2.end() && s1.dist() == s2.dist();
}
