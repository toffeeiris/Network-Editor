#include "door.h"

Door::Door(QGraphicsObject* parent)
    : RoomLayout(RoomObject::objectType::Door, parent)
{
    this->objName = "Дверь";
    this->obType = RoomObject::objectType::Door;
    this->setZValue(1);
}

Door* Door::clone() const
{
    Door* w = new Door();
    w->setPolygon(this->getPolygon());
    w->setZValue(this->zValue());
    w->setPos(this->pos());
    return w;
}

QPixmap Door::objectPixmap() const
{
    QPixmap pixmap(1, 1);
    pixmap.fill(Qt::transparent);
    return pixmap;
}

// Метод отрисовки двери
void Door::drawObject(QPainter* painter) const
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::gray);
    painter->drawPolygon(polygon);
}


// Метод, возвращающий прямоугольник, ограничивающий полигон двери
QRectF Door::boundingRect() const
{
    return polygon.boundingRect();
}

