#include <QDebug>
#include "field.h"
namespace{
    const static QColor COLOR_BLUE(210,210,255);
    const static QColor COLOR_YELLOW(241,231,36);
    const static QColor COLOR_PINK(255,63,149);
    const static QColor COLOR_GREEN(105,255,0);
    const static QColor COLOR_ORANGE(255,170,85);
}

Field::Field(QQuickItem *parent): QQuickPaintedItem(parent), pixmap(nullptr), pen(Qt::white,1) {
    connect(City::instance(),SIGNAL(needDraw()),this,SLOT(draw()));
    pixmap = new QPixmap(QSize(640, 480));
    pixmapPainter.begin(pixmap);
    area = QRect(0,0,640,480);
}

void Field::draw() {
    pixmap->fill(COLOR_BLUE);
    pixmapPainter.strokePath(painterPath, pen);
    fillField();
    this->update(area);
}

void Field::paint(QPainter* painter){
    painter->drawPixmap(area,*pixmap);
}

void Field::paintStore(const QColor &color, qreal x, qreal y) {
    static float radius = 5;
    pixmapPainter.setBrush(QBrush(color));
    pixmapPainter.setPen(Qt::NoPen);
    pixmapPainter.drawEllipse(x * 50 - radius, y * 50 - radius,2 * radius, 2 * radius);
}

void Field::paintDriver(const QColor &color, qreal x, qreal y) {
    static float radius = 5;
    pixmapPainter.setBrush(QBrush(color));
    pixmapPainter.setPen(Qt::NoPen);
    pixmapPainter.drawEllipse(x * 50 - radius, y * 50 - radius,2 * radius, 2 * radius);
}

void Field::fillField() {
//    for (int i = 0; i < City::instance()->storeNum(); i++) {
//        paintStore(COLOR_PINK, City::instance()->store(i).x(), City::instance()->store(i).y());
//    }
//    for (int i = 0; i < City::instance()->driverNum(); i++) {
//        paintDriver(COLOR_ORANGE, City::instance()->driver(i).x(), City::instance()->driver(i).y());
//    }
}
