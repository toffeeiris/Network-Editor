#ifndef COMPUTER_H
#define COMPUTER_H

#include "network_device.h"

class Computer : public NetworkDevice
{
    Q_OBJECT;
public:
    explicit Computer(QGraphicsObject *parent = 0);  // Конструктор
    ~Computer();  // Деструктор
    Computer* clone() const override;  // Метод для создания копии объекта
    QPolygonF scenePolygon() const override;  // Возвращает полигон объекта в координатах сцены

private:
    // Возвращает изображение объекта
    QPixmap objectPixmap() const override;
    // Отрисовывает объект на painter
    void drawObject(QPainter *painter) const override;

    // Статический метод для получения общего изображения компьютера
    static QPixmap& getCommonPixmap();
};

#endif // COMPUTER_H
