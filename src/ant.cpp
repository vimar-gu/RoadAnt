#include "ant.h"
#include <algorithm>
#include "utils.h"
#include <QDebug>

vector<CTarget> Ant::dealwithData(CDriver& d, vector<CPack> packList) {

    vector<CPack> driverList;
    for (auto elem : d.pickingPacks()) {
        driverList.push_back(elem);
    }
    for (auto elem : d.holdingPacks()) {
        driverList.push_back(elem);
    }
    vector<CTarget> targetList = dealwithHolding(d, driverList);

    dealwithWaiting(d, targetList, packList);

    if (!targetList.empty()) targetList.erase(targetList.begin());
    return targetList;
}

vector<CTarget> Ant::dealwithHolding(CDriver& d, vector<CPack> driverList) {
    vector<vector<CTarget>> routeRec;
    vector<CTarget> bestRec;
    vector<double> costRec;
    double minCost = 10000;
    routeRec.reserve(_antNum);
    _targetNum = d.holdingPacks().size() + d.pickingPacks().size() * 2;
    _routeNum = _targetNum * (_targetNum + 1) / 2;
    _routeTau.reserve(_routeNum);

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

            while (targetList.size() < _targetNum + 1 && !allPackList.empty()) {
                int minDist = 10000;
                CPack nextPack;
                CTarget compareTarget;
                CTarget lastTarget;
                for (auto elem : allPackList) {
                    if (!targetList.empty()) lastTarget = targetList.back();
                    if (elem.state() == 2) compareTarget = elem.destination(); // holding
                    else compareTarget = elem.source(); // picking
                    if (distBetween(lastTarget, compareTarget) < minDist) { // waited to be modified
                        minDist = distBetween(lastTarget, compareTarget);
                        nextPack = elem;
                    }
                }

                if (nextPack.source().pos().valid()) {
                    if (nextPack.state() == 2) {
                        auto pos = find_if(allPackList.begin(), allPackList.end(), [=](CPack p)
                            {return p.destination() == nextPack.destination();});
                        targetList.push_back(nextPack.destination());
                        allPackList.erase(pos);
                    }
                    else {
                        for (auto pos = allPackList.begin(); pos != allPackList.end(); pos++) {
                            if (pos->source() == nextPack.source()) {
                                targetList.push_back(nextPack.source());
                                pos->setStateHold();
                            }
                        }
                    }
                }
            }

            double cost = calcCost(targetList, allPackList);
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
    int costTime = 0;
    for (auto pos = targetList.begin(); pos != targetList.end() - 1; pos ++) {
        costTime += distBetween(*pos, *(pos+1));
    }
    for (auto& elem : packList) {
        if (costTime + calcTime(elem, *(targetList.end()))> elem.limit()) continue;
        targetList.push_back(elem.source());
        targetList.push_back(elem.destination());
    }
}
