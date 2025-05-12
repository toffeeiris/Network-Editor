#ifndef ROUTER_H
#define ROUTER_H

#include "network_device.h"

class Router : public NetworkDevice
{
    Q_OBJECT;
public:
    explicit Router(QGraphicsObject *parent = nullptr);  // Конструктор
    ~Router();  // Деструктор
    Router* clone() const override;  // Метод для создания копии объекта
    QPolygonF scenePolygon() const override;  // Возвращает полигон объекта в координатах сцены

private:
    // Возвращает изображение объекта
    QPixmap objectPixmap() const override;
    // Отрисовывает объект на painter
    void drawObject(QPainter *painter) const override;

    // Статический метод для получения общего изображения маршрутизатора
    static QPixmap& getCommonPixmap();
};

#endif // ROUTER_H
