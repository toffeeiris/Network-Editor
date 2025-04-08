#ifndef NETWORK_DEVICE_H
#define NETWORK_DEVICE_H

#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QDialog>
#include <QString>
using namespace std;

/*Список устройств:
 * сервер
 * компьютер
 * принтер
 * IP-телефон
 * маршрутизатор (роутер)
 * точка доступа wi-fi
 * IP-камеры
 */

class Network_device
{    
private:

    struct all_features
    {
        QString device_name;
        QString producer_name;
        QString device_model;
        qreal width;
        qreal length;
        qreal weight;
        qreal x;
        qreal y;
        qreal room_x;
        qreal room_y;
        int serial_number;
        int service_life;
        QString description;
    };


public:
    Network_device(const QString &device_name, qreal x, qreal y);
    void set_features(all_features curr);
};

#endif // NETWORK_DEVICE_H
