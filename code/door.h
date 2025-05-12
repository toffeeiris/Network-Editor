#ifndef DOOR_H
#define DOOR_H
#include "room_layout.h"

class Door : public RoomLayout
{
public:
    explicit Door(QGraphicsObject* parent = nullptr);
    Door* clone() const override; // Метод для создания копии объекта
    QPixmap objectPixmap() const override; // Метод возвращает иконку объекта
private:
    void drawObject(QPainter* painter) const override; //Метод отрисовки двери
    QRectF boundingRect() const override; // Метод, возвращает ограничивающий прямоугольник объекта
};

#endif // DOOR_H
