#ifndef __FIELD_H__
#define __FIELD_H__

#include <QQuickPaintedItem>
#include <QPainter>
#include <QPainterPath>
#include <QImage>
#include "city.h"
class Field : public QQuickPaintedItem{
    Q_OBJECT
public:
    void paint(QPainter* painter) override;
    Field(QQuickItem *parent = 0);
signals:
public slots:
    void draw();
private:
    void initField();
    void paintDriver(const QColor& color,qreal x,qreal y);
    void paintStore(const QColor& color,qreal x,qreal y);
    void fillField();
    QPixmap *pixmap;
    QPainter pixmapPainter;
    QPainterPath painterPath;
    QPen pen;
    QRect area;
};

#endif // __FIELD_H__
