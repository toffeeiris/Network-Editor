#include "network_device.h"
#include "device_settings_dialog.h"

NetworkDevice::NetworkDevice(QGraphicsObject *parent, RoomObject::objectType type)  : RoomObject(parent), deviceId(QUuid::createUuid())
{
    setZValue(2); // Устанавливает порядок отрисовки
    setObjectType(type);
}

// Установка характеристик устройства
void NetworkDevice::setFeatures(const Features& features)
{
    devFeatures = features;
}

// Получение характеристик устройства
NetworkDevice::Features NetworkDevice::getFeatures() const
{
    return devFeatures;
}

// Обработка двойного клика мыши
void NetworkDevice::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) // Проверка левой кнопки мыши
    {
        QWidget *parentWidget = nullptr;
        parentWidget = scene()->views().first()->window();// Получает окно первого представления сцены

        DeviceSettingsDialog *dialog = new DeviceSettingsDialog(this, parentWidget);
        dialog->exec();
    }
    RoomObject::mouseDoubleClickEvent(event);

}

QDomElement NetworkDevice::Features::toXml(QDomDocument& doc) const { // Создаёт XML-элемент с атрибутами устройства (производитель, модель и т.д.)
    QDomElement devEl = doc.createElement("device"); // Создаёт элемент <device>
    devEl.setAttribute("producer", producerName); //Устанавливает атрибут производителя
    devEl.setAttribute("model", deviceModel);
    devEl.setAttribute("serial", serialNumber);
    devEl.setAttribute("life", serviceLife);
    devEl.setAttribute("description", description);
    return devEl;
}

void NetworkDevice::Features::fromXml(const QDomElement& elem) { // Извлекает атрибуты устройства из XML-элемента
    producerName = elem.attribute("producer", "");  // Извлекает производителя, возвращает пустую строку, если атрибут отсутствует
    deviceModel = elem.attribute("model", "");
    serialNumber = elem.hasAttribute("serial") ? elem.attribute("serial").toInt() : 0;
    serviceLife = elem.hasAttribute("life") ? elem.attribute("life").toInt() : 0;
    description = elem.attribute("description", "");
}

void NetworkDevice::appendXmlExtras(QDomElement& parent, QDomDocument& doc) const { // Включает характеристики устройства в XML-структуру
    parent.appendChild(devFeatures.toXml(doc)); // Добавляет XML-элемент с характеристиками в родительский элемент
}

void NetworkDevice::loadFromXmlElement(const QDomElement& deviceEl){ // Извлекает характеристики устройства из XML-элемента
    devFeatures.fromXml(deviceEl);  // Загружает характеристики устройства из переданного XML-элемента
}
