#ifndef FURNITURE_H
#define FURNITURE_H

#include "room_object.h"

class Furniture : public RoomObject
{
public:
    explicit Furniture(QGraphicsObject *parent, RoomObject::objectType type);
};

#endif // FURNITURE_H
