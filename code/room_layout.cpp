#include "room_layout.h"

RoomLayout::RoomLayout(RoomObject::objectType type, QGraphicsObject* parent)
    : RoomObject(type, parent)
{}

// Устанавливает многоугольник и обновляет отрисовку объекта
void RoomLayout::setPolygon(const QPolygonF& p)
{
    polygon = p;
    update();
}


// Возвращает локальный многоугольник
QPolygonF RoomLayout::getPolygon() const
{
    return polygon;
}

// Возвращает многоугольник, где все точки переведены в координаты сцены
QPolygonF RoomLayout::scenePolygon() const
{
    QPolygonF result;
    for (const QPointF& pt : polygon)
        result << mapToScene(pt); // Преобразуем каждую точку в координаты сцены
    return result;
}



