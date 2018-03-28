#include "ant.h"
#include <algorithm>
#include "utils.h"
#include <QDebug>

CTarget Ant::dealwithData(CDriver& d, vector<CPack> packList) {

    vector<CPack> driverList;
    for (auto elem : d.pickingPacks()) {
        qDebug() << "pick";
        driverList.push_back(elem);
    }
    for (auto elem : d.holdingPacks()) {
        qDebug() << "hold";
        driverList.push_back(elem);
    }
    vector<CTarget> targetList = dealwithHolding(d, driverList);
    dealwithWaiting(d, targetList, packList);

    CTarget nextTarget;
    targetList.erase(targetList.begin());
    if (!targetList.empty()) nextTarget = targetList.front();
    else nextTarget = CTarget();
    qDebug() << nextTarget.pos()._x << nextTarget.pos()._y;

    return nextTarget;
}

vector<CTarget> Ant::dealwithHolding(CDriver& d, vector<CPack> driverList) {
    vector<vector<CTarget>> routeRec;
    vector<CTarget> bestRec;
    vector<double> costRec;
    double minCost = 10000;
    routeRec.resize(_antNum);
    _targetNum = driverList.size();
    _routeNum = _targetNum * (_targetNum + 1) / 2;
    _routeTau.resize(_routeNum);

    for (int cnt = 0; cnt < _maxLoop; cnt++) {

        for (auto& elem : routeRec) {
            elem.clear();
        }
        routeRec.clear();

        for (int i = 0; i < _antNum; i++) {
            vector<CTarget> targetList;
            vector<CPack> allPackList = driverList;
            CRoad driverRoad(d.start(), d.end(), d.level());
            CTarget driverTarget(driverRoad, d.dist());
            targetList.push_back(driverTarget);

            while (targetList.size() < _targetNum + 1) {
                int minDist = 10000;
                CPack nextPack;
                CTarget compareTarget;
                CTarget lastTarget;
                vector<CPack>::iterator pos;
                for (auto elem : allPackList) {
                    lastTarget = targetList.back();
                    if (elem.state() == 2) compareTarget = elem.destination(); // holding
                    else compareTarget = elem.source(); // picking
                    if (distBetween(lastTarget, compareTarget) < minDist) { // waited to be modified
                        minDist = distBetween(lastTarget, compareTarget);
                        nextPack = elem;
                    }
                }
                if (nextPack.state() == 2) {
                    pos = find_if(allPackList.begin(), allPackList.end(), [=](CPack p)
                        {return p.destination() == nextPack.destination();});
                    allPackList.erase(pos);
                    targetList.push_back(nextPack.destination());
                }
                else {
                    pos = find_if(allPackList.begin(), allPackList.end(), [=](CPack p)
                        {return p.destination() == nextPack.source();});
                    (*pos).setStateHold();
                    targetList.push_back(nextPack.source());
                }
            }

            double cost = calcCost(targetList);
            costRec.push_back(cost);
            routeRec.push_back(targetList);
        }

        for (int elem = 0; elem < _antNum; elem++) {
            if (costRec.at(elem) < minCost) {
                minCost = costRec.at(elem);
                bestRec = routeRec.at(elem);
            }
        }
    }
    return bestRec;
}

void Ant::dealwithWaiting(CDriver &d, vector<CTarget> &targetList, vector<CPack> packList) {
    for (auto& elem : packList) {
        targetList.push_back(elem.source());
        targetList.push_back(elem.destination());
    }
}
