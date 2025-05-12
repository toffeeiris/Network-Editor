#ifndef ARMCHAIR_H
#define ARMCHAIR_H

#include "furniture.h"

class Armchair : public Furniture
{
    Q_OBJECT;

public:
    explicit Armchair(QGraphicsObject *parent = 0);  // Конструктор
    ~Armchair();  // Деструктор

    Armchair* clone() const override;  // Создание копии объекта
    QPolygonF scenePolygon() const override;  // Получение полигона в координатах сцены

private:
    // Методы для работы с графическим представлением
    QPixmap objectPixmap() const override;  // Получение изображения кресла
    void drawObject(QPainter *painter) const override;  // Отрисовка кресла

    // Статический метод для получения общего изображения всех кресел
    static QPixmap& getCommonPixmap();
};

#endif // ARMCHAIR_H
