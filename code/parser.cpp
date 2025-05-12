#include "parser.h"

#include "parser.h"
#include "wall.h"
#include "door.h"
#include "window.h"

#include "cabinet.h"
#include "chair.h"
#include "armchair.h"
#include "table.h"

#include "computer.h"
#include "server.h"
#include "printer.h"
#include "ip_camera.h"
#include "ip_phone.h"
#include "router.h"
Parser::Parser(QObject* parent) : QObject(parent) {}

QString Parser::loadXML(const QString& fileName, QGraphicsScene* scene)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return "Файл не найден";
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        return "Ошибка при разборе XML.";
    }

    // Получаем корневой элемент <room>
    QDomElement root = doc.documentElement();
    QDomNodeList allNodes = root.childNodes();

    // Проходим по всем группам объектов (<walls>, <doors>, <computers> и т.д.)
    for (int i = 0; i < allNodes.count(); ++i) {
        QDomElement wrapper = allNodes.at(i).toElement();
        QDomNodeList objs = wrapper.childNodes();
        // Проходим по каждому отдельному объекту внутри группы
        for (int j = 0; j < objs.count(); ++j) {
            QDomElement elem = objs.at(j).toElement();

            // Определяем тип объекта по имени тега (например <door>)
            RoomObject::objectType type = RoomObject::stringToType(elem.tagName());
                // Создаём объект и задаём ему форму
            RoomObject* obj = parseLayoutElement(elem, type);

            if (obj) {
                // Если объект типа NetworkDevice, загружаем его параметры
                if (auto netDev = dynamic_cast<NetworkDevice*>(obj)) {

                    QDomElement deviceEl = elem.firstChildElement("device");
                    if (!deviceEl.isNull()) {
                        netDev->loadFromXmlElement(deviceEl);  // Загружаем параметры устройства
                    }
                }

                scene->addItem(obj); // Добавляем объект на сцену
            }
        }
    }

    return "ОК";
}

RoomObject* Parser::parseLayoutElement(const QDomElement& element, RoomObject::objectType type)
{

    QPolygonF polygon;
    QDomNodeList points = element.firstChildElement("polygon").elementsByTagName("point");

    // Чтение всех координат <point x="..." y="...">
    for (int i = 0; i < points.count(); ++i) {
        QDomElement pt = points.at(i).toElement();
        polygon << QPointF(pt.attribute("x").toDouble(), pt.attribute("y").toDouble());
    }

    RoomObject* obj = nullptr;

    // Создаём объект нужного типа
    switch (type) {
    case RoomObject::objectType::Wall:      obj = new Wall(); break;
    case RoomObject::objectType::Door:      obj = new Door(); break;
    case RoomObject::objectType::Window:    obj = new Window(); break;
    case RoomObject::objectType::Computer:  obj = new Computer(); break;
    case RoomObject::objectType::Table:     obj = new Table(); break;
    case RoomObject::objectType::Cabinet:   obj = new Cabinet(); break;
    case RoomObject::objectType::Chair:     obj = new Chair(); break;
    case RoomObject::objectType::Armchair:  obj = new Armchair(); break;
    case RoomObject::objectType::Server:    obj = new Server(); break;
    case RoomObject::objectType::Printer:   obj = new Printer(); break;
    case RoomObject::objectType::IPPhone:   obj = new IPPhone(); break;
    case RoomObject::objectType::Router:    obj = new Router(); break;
    case RoomObject::objectType::IPCamera:  obj = new IPCamera(); break;
    default: return nullptr;

        //Если это объект с геометрией — применяем polygon
        if (auto layout = dynamic_cast<RoomLayout*>(obj)) {
            layout->setPolygon(polygon);
        }
        // Иначе, если это мебель или устройство, позиционируем по прямоугольнику
        else if (polygon.size() >= 2) {
            QRectF bounds = polygon.boundingRect();
            obj->setSize(bounds.width(), bounds.height());
            obj->setPos(bounds.topLeft());
        }
    }


    // Если это объект с геометрией — применяем polygon
    if (auto layout = dynamic_cast<RoomLayout*>(obj)) {
        layout->setPolygon(polygon);
    } else if (polygon.size() >= 2) {
        QRectF bounds = polygon.boundingRect();
        obj->setSize(bounds.width(), bounds.height());
        obj->setPos(bounds.topLeft());
    }

    return obj;
}

bool Parser::saveXML(const QString& fileName, const QList<QGraphicsItem*>& items)
{

    QDomDocument doc;
    QDomElement root = doc.createElement("room");
    root.setAttribute("width", 800);
    root.setAttribute("height", 600);
    doc.appendChild(root);

    QMap<QString, QDomElement> sections;

    // Группируем объекты по типам (все стены — в <walls>, все компьютеры — в <computers>)
    for (QGraphicsItem* item : items) {
        RoomObject* obj = dynamic_cast<RoomObject*>(item);
        if (!obj) continue;

        QString tag = RoomObject::typeToString(obj->getObjectType());
        QString sectionTag = tag + "s";

        if (!sections.contains(sectionTag)) // Создаём контейнер (если его ещё нет)
            sections[sectionTag] = doc.createElement(sectionTag);

        // Описываем объект
        QDomElement objElement = doc.createElement(tag);
        QDomElement polygonEl = doc.createElement("polygon");


        // Записываем координаты полигона
        for (const QPointF& pt : obj->scenePolygon()) {
            QDomElement ptEl = doc.createElement("point");
            ptEl.setAttribute("x", pt.x());
            ptEl.setAttribute("y", pt.y());
            polygonEl.appendChild(ptEl);
        }

        objElement.appendChild(polygonEl);

        // Добавляем данные сетевого устройства
        if (const NetworkDevice* netDev = dynamic_cast<const NetworkDevice*>(obj)) {
            netDev->appendXmlExtras(objElement, doc);
        }

        sections[sectionTag].appendChild(objElement);
    }

    // Добавляем все секции в корневой элемент <room>
    for (auto it = sections.begin(); it != sections.end(); ++it)
        root.appendChild(it.value());

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    doc.save(out, 4); // Сохраняем XML с отступами
    return true;
}


