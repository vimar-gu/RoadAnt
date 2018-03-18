#include "ant.h"

void Ant::dealwithData(CDriver& d, vector<CPack> packList) {
    int targetNum = packList.size();
    int waitingNum = 0;
    for (auto& elem : packList) {
        if (elem.state() == 0) {
            waitingNum++;
        }
    }
    targetNum += waitingNum;
    _routeNum = targetNum * (targetNum - 1) / 2;
    _routeTau.resize(_routeNum);

    vector<vector<int>> randPosList;
    randList(randPosList);

    int vel;
    d.setVel(vel);
}

void Ant::randList(vector<vector<int>> L) {
    vector<int> list;
    vector<int> tmpList;
}
