#include "ant.h"

vector<CPack> Ant::dealwithData(CDriver& d, vector<CPack> packList) {
    vector<CPack> targetPackList;


    _targetNum = packList.size();
    int waitingNum = 0;
    for (auto& elem : packList) {
        if (elem.state() == 0) {
            waitingNum++;
        }
    }
    _targetNum += waitingNum;
    _routeNum = _targetNum * (_targetNum - 1) / 2;
    _routeTau.resize(_routeNum);

    vector<vector<int>> randPosList;
    randPosList.resize(_antNum);
    if (_isOrigin) {
        randOrigin(randPosList, packList);
        _isOrigin = 1;
    }
    else randList(randPosList, packList);


    return targetPackList;
}

void Ant::randOrigin(vector<vector<int>>& list, vector<CPack> packList) {
    vector<CTarget> targetList;
    targetList.resize(_targetNum);

    for (auto& elem : packList) {
        if (elem.state() == 0) targetList.push_back(elem.source());
        targetList.push_back(elem.destination());
    }
    for (int i = 0; i < _antNum; i++) {
        int tmp = rand() % packList.size();
        CPack tmpPack = packList.at(tmp);
        if (tmpPack.state() == 1) {
//            targetList.push_back(tmpPack);
//            packList.erase()
        }
    }
}

void Ant::randList(vector<vector<int>>& list, vector<CPack> packList) {

}
