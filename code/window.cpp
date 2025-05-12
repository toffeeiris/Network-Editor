#include "window.h"

Window::Window(QGraphicsObject* parent)
    : RoomLayout(RoomObject::objectType::Window, parent)
{
    this->objName = "Окно";
    this->obType = RoomObject::objectType::Window;
    this->setZValue(1);
}


Window* Window::clone() const
{
    Window* w = new Window();
    w->setPolygon(this->getPolygon());
    w->setZValue(this->zValue());
    w->setPos(this->pos());
    return w;
}

QPixmap Window::objectPixmap() const
{
    QPixmap pixmap(1, 1);
    pixmap.fill(Qt::transparent);
    return pixmap;
}

// Метод отрисовки окна
void Window::drawObject(QPainter* painter) const
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::darkCyan);
    painter->drawPolygon(polygon);
}

// Метод, возвращающий прямоугольник, ограничивающий полигон окна
QRectF Window::boundingRect() const
{
    return polygon.boundingRect();
}
