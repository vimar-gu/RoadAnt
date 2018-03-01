#ifndef INTERACTION_H
#define INTERACTION_H
#include <QObject>

class Interaction : public QObject
{
    Q_OBJECT
public:
    Interaction() {}
    Q_INVOKABLE void setVision();
};

#endif // INTERACTION_H
