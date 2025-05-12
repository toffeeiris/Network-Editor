#include "furniture.h"

// Конструктор по умолчанию
Furniture::Furniture(QGraphicsObject *parent, RoomObject::objectType type): RoomObject(parent)
{
    setZValue(1);
    setObjectType(type);
    this->arType = OnFloor;
}
