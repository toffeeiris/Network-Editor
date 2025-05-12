#ifndef ROOMLAYOUT_H
#define ROOMLAYOUT_H

#include "room_object.h"
#include <QPolygonF>

class RoomLayout : public RoomObject
{
public:
    explicit RoomLayout(RoomObject::objectType type, QGraphicsObject* parent);

    void setPolygon(const QPolygonF& polygon); // Устанавливает форму объекта (многоугольник)
    QPolygonF getPolygon() const override; // Возвращает локальный многоугольник
    QPolygonF scenePolygon() const override; // Возвращает многоугольник, преобразованный в координаты сцены

protected:
    QPolygonF polygon; // Многоугольник, задающий форму объекта
};

#endif // ROOMLAYOUT_H


