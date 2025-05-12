#ifndef PARSER_H
#define PARSER_H

#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include "room_object.h"

class Parser : public QObject
{
public:
    explicit Parser(QObject* parent = nullptr);

    // Загружает файл и добавляет все объекты в сцену
    QString loadXML(const QString& fileName, QGraphicsScene* scene);
    // Сохраняет объекты сцены в XML-файл
    bool saveXML(const QString& fileName, const QList<QGraphicsItem*>& items);

private:
    // Парсит один XML-элемент и возвращает соответствующий объект
    RoomObject* parseLayoutElement(const QDomElement& element, RoomObject::objectType type);
};

#endif // PARSER_H
