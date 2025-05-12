#ifndef WINDOW_H
#define WINDOW_H

#include "room_layout.h"

class Window : public RoomLayout
{
public:
    explicit Window(QGraphicsObject* parent = nullptr);
    Window* clone() const override; // Метод для создания копии объекта
    QPixmap objectPixmap() const override; // Метод возвращает иконку объекта
private:
    void drawObject(QPainter* painter) const override; //Метод отрисовки двери
    QRectF boundingRect() const override; // Метод, возвращает ограничивающий прямоугольник объекта
};


#endif // WINDOW_H
