#ifndef UTILS_H
#define UTILS_H
#include "city.h"

bool operator ==(const CPos p1, const CPos p2);
bool operator ==(const CStore s1, const CStore s2);
bool operator ==(const CPack& p1, const CPack p2);
bool isOntheRight(const CDriver& d, const CTarget& t);
int distBetween(CTarget d, CTarget c);
int distBetween(CDriver d, CTarget c);
int distBetween(CDriver d, CPos p);
int getMinDist(CDriver d, CTarget t);
double calcCost(vector<CTarget> targetList);
int calcTime(CPack p, CTarget t);

#endif // UTILS_H
