#ifndef PRINTER_H
#define PRINTER_H

#include "network_device.h"

class Printer : public NetworkDevice
{
    Q_OBJECT;
public:
    explicit Printer(QGraphicsObject *parent = 0);  // Конструктор
    ~Printer();  // Деструктор
    Printer* clone() const override;  // Метод для создания копии объекта
    QPolygonF scenePolygon() const override;  // Возвращает полигон объекта в координатах сцены

private:
    // Возвращает изображение объекта
    QPixmap objectPixmap() const override;
    // Отрисовывает объект на painter
    void drawObject(QPainter *painter) const override;

    // Статический метод для получения общего изображения принтера
    static QPixmap& getCommonPixmap();
};

#endif // PRINTER_H
