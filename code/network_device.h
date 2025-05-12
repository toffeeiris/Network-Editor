#ifndef NETWORK_DEVICE_H
#define NETWORK_DEVICE_H

#include "room_object.h"

#include <QUuid>
#include <QDomDocument>
#include <QApplication>
#include <QFile>

class DeviceSettingsDialog;
using namespace std;

class NetworkDevice : public RoomObject
{
protected:

    struct Features
    {
        QString producerName = ""; // Название производителя
        QString deviceModel = ""; // Модель устройства
        int serialNumber = 0; // Серийный номер
        int serviceLife = 0; // Срок службы
        QString description = ""; // Название устройства


        QDomElement toXml(QDomDocument& doc) const; // Метод для сериализации в XML
        void fromXml(const QDomElement& elem); // Метод для десериализации из XML
    };

    Features devFeatures; // Характеристики устройства
    QUuid deviceId; // Уникальный идентификатор устройства
    friend class DeviceSettingsDialog;


public:
    explicit NetworkDevice(QGraphicsObject *parent, RoomObject::objectType type);

    void setFeatures(const Features&);
    Features getFeatures() const;

    QUuid getDeviceId() const { return deviceId; }

    QString getProducerName() const { return devFeatures.producerName; }
    QString getDeviceModel() const { return devFeatures.deviceModel; }
    int getSerialNumber() const { return devFeatures.serialNumber; }
    int getServiceLife() const { return devFeatures.serviceLife; }
    QString getDescription() const { return devFeatures.description; }

    void setProducerName(const QString &name) { devFeatures.producerName = name; }
    void setDeviceModel(const QString &model) { devFeatures.deviceModel = model; }
    void setSerialNumber(int number) { devFeatures.serialNumber = number; }
    void setServiceLife(int life) { devFeatures.serviceLife = life; }
    void setDescription(const QString &desc) { devFeatures.description = desc; }

    void appendXmlExtras(QDomElement& parent, QDomDocument& doc) const;
    void loadFromXmlElement(const QDomElement& deviceEl);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;  // Переопределение обработки события двойного клика мыши
};

#endif // NETWORK_DEVICE_H
