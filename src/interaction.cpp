#include "interaction.h"
#include "city.h"

Interaction::Interaction(QObject *parent) : QObject(parent) {

}

void Interaction::setVision(){
    City::instance()->start();
}

int Interaction::finishedNum(){
    return City::instance()->finishedCase();
}

int Interaction::currentTime() {
    return City::instance()->currentTime();
}

Interaction::~Interaction() {
}
