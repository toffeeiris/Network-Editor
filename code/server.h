#ifndef SERVER_H
#define SERVER_H

#include "network_device.h"

class Server : public NetworkDevice
{
    Q_OBJECT;
public:
    explicit Server(QGraphicsObject *parent = 0);  // Конструктор
    ~Server();  // Деструктор
    Server* clone() const override;  // Метод для создания копии объекта
    QPolygonF scenePolygon() const override;  // Возвращает полигон объекта в координатах сцены

private:
    // Возвращает изображение объекта
    QPixmap objectPixmap() const override;
    // Отрисовывает объект на painter
    void drawObject(QPainter *painter) const override;

    // Статический метод для получения общего изображения сервера
    static QPixmap& getCommonPixmap();
};

#endif // SERVER_H
