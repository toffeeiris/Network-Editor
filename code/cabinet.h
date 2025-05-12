#ifndef CABINET_H
#define CABINET_H

#include "furniture.h"

class Cabinet : public Furniture
{
    Q_OBJECT;

public:
    explicit Cabinet(QGraphicsObject *parent = 0); // Конструктор
    ~Cabinet(); // Деструктор

    Cabinet* clone() const override; // Метод для создания копии объекта
    QPolygonF scenePolygon() const override; // Получение полигона в координатах сцены

private:
    // Методы для работы с графическим представлением
    QPixmap objectPixmap() const override; // Генерация изображения тумбы
    void drawObject(QPainter *painter) const override; // Отрисовка тумбы

protected:
    QColor color = QColor(101, 67, 33); // Основной цвет тумбы (коричневый)
    QColor border = QColor(Qt::black); // Цвет границы тумбы
};

#endif // CABINET_H
