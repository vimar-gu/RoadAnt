#ifndef UTILS_H
#define UTILS_H
#include "city.h"

bool operator ==(const CPos p1, const CPos p2);
bool operator ==(const CStore s1, const CStore s2);
bool operator ==(const CPack& p1, const CPack p2);
int distBetween(CTarget d, CTarget c);
int distBetween(CDriver d, CTarget c);
double calcCost(vector<CTarget> targetList);

#endif // UTILS_H
