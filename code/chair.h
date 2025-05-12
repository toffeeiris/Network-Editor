#ifndef CHAIR_H
#define CHAIR_H

#include "furniture.h"

class Chair : public Furniture
{
    Q_OBJECT;

public:
    explicit Chair(QGraphicsObject *parent = 0); // Конструктор
    ~Chair(); // Деструктор

    Chair* clone() const override; // Создание копии объекта
    QPolygonF scenePolygon() const override; // Получение полигона в координатах сцены

private:
    // Методы для работы с графическим представлением
    QPixmap objectPixmap() const override; // Генерация изображения стула
    void drawObject(QPainter *painter) const override; // Отрисовка стула

protected:
    QColor color = QColor(204, 119, 34); // Основной цвет стула (оранжево-коричневый)
    QColor border = QColor(101, 67, 33); // Цвет границы (темно-коричневый)
};

#endif // CHAIR_H
