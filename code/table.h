#ifndef TABLE_H
#define TABLE_H

#include "furniture.h"

class Table : public Furniture
{
    Q_OBJECT;

public:
    explicit Table(QGraphicsObject *parent = 0); // Конструктор
    ~Table(); // Деструктор

    Table* clone() const override; // Создание копии объекта
    QPolygonF scenePolygon() const override; // Получение полигона в координатах сцены

private:
    // Методы для работы с графическим представлением
    QPixmap objectPixmap() const override; // Генерация изображения стола
    void drawObject(QPainter *painter) const override; // Отрисовка стола

protected:
    QColor color = QColor(101, 67, 33); // Основной цвет стола (коричневый)
    QColor border = QColor(Qt::black); // Цвет границы (черный)
};

#endif // TABLE_H
