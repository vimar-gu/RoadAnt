#include "ant.h"
#include <algorithm>
#include "utils.h"
#include <QDebug>

CTarget Ant::dealwithData(CDriver& d, vector<CPack> packList) {

    vector<CTarget> driverList;
    for (auto& elem : d.pickingPacks()) {
        qDebug() << "pick";
        driverList.push_back(elem.destination());
    }
    for (auto& elem : d.holdingPacks()) {
        qDebug() << "hold";
        driverList.push_back(elem.source());
        driverList.push_back(elem.destination());
    }
    vector<CTarget> targetList = dealwithHolding(d, driverList);
    dealwithWaiting(d, targetList, packList);

    CTarget nextTarget;
    targetList.erase(targetList.begin());
    if (!targetList.empty()) nextTarget = targetList.front();
    else nextTarget = CTarget();

    return nextTarget;
}

vector<CTarget> Ant::dealwithHolding(CDriver& d, vector<CTarget> driverList) {
    vector<vector<CTarget>> routeRec;
    vector<CTarget> bestRec;
    vector<double> costRec;
    double minCost = 10000;
    routeRec.resize(_antNum);
    _targetNum = driverList.size();
    _routeNum = _targetNum * (_targetNum - 1) / 2;
    _routeTau.resize(_routeNum);

    for (int cnt = 0; cnt < _maxLoop; cnt++) {

        for (auto& elem : routeRec) {
            elem.clear();
        }
        routeRec.clear();

        for (int i = 0; i < _antNum; i++) {
            vector<CTarget> targetList;
            vector<CTarget> allTargetList = driverList;
            CRoad driverRoad(d.start(), d.end(), d.level());
            CTarget driverTarget(driverRoad, d.dist());
            targetList.push_back(driverTarget);

            while (targetList.size() < _targetNum + 1) {
                int minDist = 10000;
                CTarget nextTarget;
                for (auto& elem : allTargetList) {
                    CTarget lastTarget = targetList.back();
                    if (distBetween(lastTarget, elem) < minDist) { // waited to be modified
                        minDist = distBetween(lastTarget, elem);
                        nextTarget = elem;
                    }
                }
                auto pos = find_if(allTargetList.begin(), allTargetList.end(), [=](CTarget t){return t == nextTarget;});
                allTargetList.erase(pos);
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
