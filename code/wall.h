#ifndef WALL_H
#define WALL_H

#include "room_layout.h"

class Wall : public RoomLayout
{
public:
    explicit Wall(QGraphicsObject* parent = nullptr);
    Wall* clone() const override; // Метод для создания копии объекта
    QPixmap objectPixmap() const override; // Метод возвращает иконку объекта
private:
    void drawObject(QPainter* painter) const override; //Метод отрисовки двери
    QRectF boundingRect() const override; // Метод, возвращает ограничивающий прямоугольник объекта
};

#endif // WALL_H
