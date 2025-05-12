#ifndef IP_PHONE_H
#define IP_PHONE_H

#include "network_device.h"

class IPPhone : public NetworkDevice
{
    Q_OBJECT;
public:
    explicit IPPhone(QGraphicsObject *parent = 0);  // Конструктор
    ~IPPhone();  // Деструктор
    IPPhone* clone() const override;  // Метод для создания копии объекта
    QPolygonF scenePolygon() const override;  // Возвращает полигон объекта в координатах сцены

private:
    // Возвращает изображение объекта
    QPixmap objectPixmap() const override;
    // Отрисовывает объект на painter
    void drawObject(QPainter *painter) const override;

    // Статический метод для получения общего изображения IP-телефона
    static QPixmap& getCommonPixmap();
};

#endif // IP_PHONE_H
