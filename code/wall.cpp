#include "wall.h"

Wall::Wall(QGraphicsObject* parent)
    : RoomLayout(RoomObject::objectType::Wall, parent)
{
    this->objName = "Стена";
    this->obType = RoomObject::objectType::Wall;
    this->setZValue(0);
}

Wall* Wall::clone() const
{
    Wall* w = new Wall();
    w->setPolygon(this->getPolygon());
    w->setZValue(this->zValue());
    w->setPos(this->pos());
    return w;
}

QPixmap Wall::objectPixmap() const
{
    QPixmap pixmap(1, 1);
    pixmap.fill(Qt::transparent);
    return pixmap;
}


// Метод отрисовки стены
void Wall::drawObject(QPainter* painter) const
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);
    painter->drawPolygon(polygon);
}

// Метод, возвращающий прямоугольник, ограничивающий полигон стены
QRectF Wall::boundingRect() const
{
    return polygon.boundingRect();
}
