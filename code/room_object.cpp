#include "room_object.h"

// Конструктор по умолчанию
RoomObject::RoomObject(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    // Устанавливаем флаги для объекта
    setFlag(ItemIsMovable);         // Можно перемещать
    setFlag(ItemIsSelectable);       // Можно выбирать
    setFlag(ItemSendsGeometryChanges); // Отправлять изменения геометрии
    setFlag(ItemSendsScenePositionChanges, true); // Отправлять изменения позиции
    setFlag(ItemIsFocusable, true);  // Может получать фокус
    setAcceptHoverEvents(true);      // Принимать события наведения
}

// Конструктор с указанием типа объекта
RoomObject::RoomObject(objectType type, QGraphicsObject* parent)
    : QGraphicsObject(parent), obType(type)
{
    // Устанавливаем те же флаги, что и в конструкторе по умолчанию
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemSendsScenePositionChanges, true);
    setFlag(ItemIsFocusable, true);
    setAcceptHoverEvents(true);
}

// Деструктор
RoomObject::~RoomObject()
{

}

// Возвращает ограничивающий прямоугольник объекта
QRectF RoomObject::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

// Отрисовка объекта
void RoomObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Если курсор над объектом, делаем его полупрозрачным
    if(option->state & QStyle::State_MouseOver)
    {
        painter->setOpacity(0.7);
    }

    // Вызываем метод отрисовки объекта
    drawObject(painter);

    // Если объект выбран, не заливаем его
    if(isSelected())
    {
        painter->setBrush(Qt::NoBrush);
    }

    // Восстанавливаем непрозрачность
    painter->setOpacity(1.0);
}

// Метод отрисовки объекта
void RoomObject::drawObject(QPainter *painter) const
{
    painter->save();

    int w = width;
    int h = height;

    // Если объект повернут на 90 или 270 градусов, меняем ширину и высоту местами
    if (angle == 90 || angle == 270)
    {
        w = height;
        h = width;
    }

    // Получаем изображение объекта и рисуем его
    QPixmap pixmap = objectPixmap();
    if (!pixmap.isNull())
    {
        painter->drawPixmap(0, 0, pixmap.scaled(w, h,
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
    }

    painter->restore();
}

// Возвращает полигон объекта в координатах сцены
QPolygonF RoomObject::scenePolygon() const
{
    QRectF rect = boundingRect();
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}

// Возвращает полигон объекта в локальных координатах
QPolygonF RoomObject::getPolygon() const
{
    QRectF rect = boundingRect();
    return QPolygonF() << rect.topLeft()
                       << rect.topRight()
                       << rect.bottomRight()
                       << rect.bottomLeft();
}

// Установка размера объекта
void RoomObject::setSize(int curr_w, int curr_h)
{
    prepareGeometryChange(); // Готовимся к изменению геометрии
    width = curr_w;
    height = curr_h;
}

// Обработчик нажатия кнопки мыши
void RoomObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (cType == containerType::ScrollArea)
        {
            // Если объект в панели инструментов, начинаем перетаскивание
            emit dragObject(this, event->scenePos());
            isDragging = true;
        }
        else if (cType == containerType::MainScene)
        {
            // Если объект на сцене и это стол или шкаф с устройствами, запрещаем перемещение
            if ((obType == objectType::Table || obType == objectType::Cabinet) && !childObjs.isEmpty())
            {
                QMessageBox::warning(nullptr, "Ошибка перемещения",
                                     QString("Нельзя переместить объект %1, так как на нем находятся устройство(а)").arg(objName));
                return;
            }

            // Сохраняем начальную позицию для перемещения
            dragStartPos = event->pos();
            validPos = pos();
            setCursor(Qt::ClosedHandCursor);
        }
    }
    else if (event->button() == Qt::RightButton && cType == containerType::MainScene)
    {
        // Правая кнопка - удаление объекта
        if (obType == objectType::Wall  || obType == objectType::Window  || obType == objectType::Door)
        {
            QMessageBox::warning(nullptr, "Ошибка удаления",
                                 QString("Нельзя удалить этот объект").arg(objName));
            return;
        }

        // Проверка возможности удаления мебели с устройствами
        if (isFurniture() && !childObjs.isEmpty())
        {
            bool canDelete = true;
            QStringList problematicDevices;

            for (RoomObject* child : childObjs)
            {
                if (child->getAreaType() != OnFloor)
                {
                    QPointF floorPos = QPointF(rand() % 500, rand() % 500);
                    if (!child->canBePlaced(floorPos))
                    {
                        canDelete = false;
                        problematicDevices << child->objName;
                    }
                }
            }

            if (!canDelete)
            {
                QMessageBox::warning(nullptr, "Ошибка удаления",
                                     QString("Нельзя удалить объект %1, так как следующие устройства не могут быть размещены на полу: %2")
                                         .arg(objName)
                                         .arg(problematicDevices.join(", ")));
                return;
            }
        }

        // Удаляем дочерние объекты
        for (RoomObject* child : childObjs)
        {
            child->attach(nullptr);
            emit deleteObject(child);
        }

        // Удаляем текущий объект
        attach(nullptr);
        emit deleteObject(this);
        return;
    }

    QGraphicsItem::mousePressEvent(event);
}

// Обработчик перемещения мыши
void RoomObject::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (cType == containerType::MainScene && (event->buttons() == Qt::LeftButton))
    {
        // Если это стол или шкаф с устройствами, запрещаем перемещение
        if ((obType == objectType::Table || obType == objectType::Cabinet) && !childObjs.isEmpty())
        {
            return;
        }

        // Вычисляем новую позицию
        QPointF newPos = mapToScene(event->pos() - dragStartPos);

        // Проверяем, чтобы объект не выходил за границы сцены
        QRectF sceneRect = scene()->sceneRect();
        QRectF itemRect = boundingRect().translated(newPos);

        if (!sceneRect.contains(itemRect))
        {
            newPos.setX(qMax(sceneRect.left(), qMin(newPos.x(), sceneRect.right() - width)));
            newPos.setY(qMax(sceneRect.top(), qMin(newPos.y(), sceneRect.bottom() - height)));
        }

        // Проверяем возможность размещения в новой позиции
        if (!canBePlaced(newPos))
        {
            setCursor(Qt::ForbiddenCursor);
            return;
        }

        // Перемещаем объект
        QPointF offset = newPos - pos();
        setPos(newPos);
        validPos = newPos;

        setCursor(Qt::ClosedHandCursor);
    }
    else if (isDragging && cType == containerType::ScrollArea)
    {
        // Если объект перетаскивается из панели инструментов
        emit dragObject(this, event->scenePos());
    }

    QGraphicsItem::mouseMoveEvent(event);
}

// Обработчик отпускания кнопки мыши
void RoomObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (cType == containerType::ScrollArea)
        {
            // Завершаем перетаскивание из панели инструментов
            emit droppedObject(this, mapToScene(event->pos()));
            isDragging = false;
        }
        else if (cType == containerType::MainScene)
        {
            // Для сетевых устройств проверяем поверхность размещения
            if (isNetworkDevice())
            {
                RoomObject* surface = nullptr;
                QPolygonF devicePoly = mapToScene(boundingRect());

                // Ищем подходящую поверхность (стол или шкаф)
                for (QGraphicsItem* item : scene()->items(devicePoly.boundingRect()))
                {
                    if (item == this) continue;

                    RoomObject* obj = dynamic_cast<RoomObject*>(item);
                    if (!obj) continue;

                    if ((arType & OnTable && obj->obType == objectType::Table) ||
                        (arType & OnCabinet && obj->obType == objectType::Cabinet))
                    {
                        QPolygonF objPoly = obj->mapToScene(obj->boundingRect());

                        if (QRegion(objPoly.toPolygon()).contains(QPoint(devicePoly.boundingRect().center().toPoint())))
                        {
                            surface = obj;
                            break;
                        }
                    }
                }

                // Прикрепляем устройство к поверхности
                attach(surface);

                // Если устройство должно быть на поверхности, но поверхность не найдена
                if (arType != OnFloor && !surface && parentObj)
                {
                    attach(nullptr);
                    deleteLater();
                    return;
                }
            }

            // Если объект нельзя разместить в текущей позиции, анимируем возврат
            if (!canBePlaced(pos()))
            {
                QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
                anim->setDuration(100);
                anim->setStartValue(pos());
                anim->setEndValue(validPos);
                anim->setEasingCurve(QEasingCurve::OutQuad);
                anim->start(QAbstractAnimation::DeleteWhenStopped);
            }
            setCursor(Qt::OpenHandCursor);
        }
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

// Проверка возможности размещения объекта в указанной позиции
bool RoomObject::canBePlaced(const QPointF& pos) const
{
    QRectF currRect = boundingRect().translated(pos);
    QPolygonF poly = mapToScene(boundingRect());

    QList<QGraphicsItem*> currItems = scene()->items();

    bool isValidArea = (arType == OnFloor); // По умолчанию можно размещать на полу
    bool nearWall = false;

    // Проверяем все объекты на сцене
    for (QGraphicsItem* item : currItems)
    {
        if (item == this || !item->isVisible())
            continue;

        RoomObject* obj = dynamic_cast<RoomObject*>(item);
        if (!obj || childObjs.contains(obj))
            continue;

        // Проверяем пересечение с другими сетевыми устройствами на той же поверхности
        if (isNetworkDevice() && obj->isNetworkDevice() &&
            parentObj && obj->parentObj == parentObj)
        {
            return false;
        }

        QRectF objRect = obj->boundingRect().translated(obj->pos());

        // Проверка для стен, дверей и окон
        if (obj->obType == objectType::Door ||
            obj->obType == objectType::Wall ||
            obj->obType == objectType::Window)
        {
            if (currRect.intersects(objRect))
                return false;

            // Проверка минимального расстояния до двери для мебели и устройств
            if ((isNetworkDevice() || isFurniture()) && obj->obType == objectType::Door)
            {
                const int minDistanceToDoor = 30;
                QRectF expandedDoorRect = objRect.adjusted(-minDistanceToDoor, -minDistanceToDoor,
                                                           minDistanceToDoor, minDistanceToDoor);
                if (expandedDoorRect.intersects(currRect))
                {
                    return false;
                }
            }

            // Проверка близости к стене
            if (obj->obType == objectType::Wall)
            {
                const int dist = 5;
                bool nearLeft = qAbs(currRect.left() - objRect.right()) <= dist;
                bool nearRight = qAbs(currRect.right() - objRect.left()) <= dist;
                bool nearTop = qAbs(currRect.top() - objRect.bottom()) <= dist;
                bool nearBottom = qAbs(currRect.bottom() - objRect.top()) <= dist;

                if (nearLeft || nearRight || nearTop || nearBottom)
                    nearWall = true;
            }
        }

        // Проверка пересечения с другими устройствами или мебелью
        if ((isNetworkDevice() && obj->isNetworkDevice()) ||
            (isFurniture() && obj->isFurniture()))
        {
            if (currRect.intersects(objRect))
                return false;
        }

        // Серверы не могут пересекаться с мебелью
        if (obType == objectType::Server && obj->isFurniture())
        {
            if (currRect.intersects(objRect))
                return false;
        }

        // Проверка, что сетевое устройство находится на поверхности
        if (isNetworkDevice() && parentObj && !obj->isFurniture())
        {
            QRectF parentRect = parentObj->boundingRect().translated(parentObj->pos());
            if (!parentRect.contains(currRect))
            {
                return false;
            }
        }

        // Проверка размещения на столе или шкафу
        if (arType != OnFloor && obType != objectType::Server)
        {
            if ((arType & OnTable) && obj->obType == objectType::Table && objRect.contains(currRect))
                isValidArea = true;

            if ((arType & OnCabinet) && obj->obType == objectType::Cabinet && objRect.contains(currRect))
                isValidArea = true;
        }

        // Проверка размещения у стены
        if ((arType & NearWall) && nearWall)
            isValidArea = true;
    }

    return isValidArea;
}


// Обработчик события колесика мыши (поворот объекта)
void RoomObject::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (cType != containerType::MainScene || !isSelected())
    {
        event->ignore();
        return;
    }

    // IP-камеры нельзя поворачивать
    if (obType == objectType::IPCamera)
    {
        QMessageBox::warning(nullptr, "Предупреждение", "Поворот IP-камер не допускается");
        event->ignore();
        return;
    }

    // Вычисляем новый угол поворота
    int delta = event->delta();
    int step = 90;
    int newAngle = (angle + (delta > 0 ? step : -step)) % 360;
    if (newAngle < 0) newAngle += 360;

    QPointF oldPos = pos();
    QPointF center = oldPos + QPointF(width, height) / 2;

    angle = newAngle;
    setTransformOriginPoint(boundingRect().center());

    // Если объект не квадратный, меняем ширину и высоту местами
    if (width != height)
    {
        prepareGeometryChange();
        int oldWidth = width;
        int oldHeight = height;
        qSwap(width, height);

        QPointF newPos = center - QPointF(width, height) / 2;

        // Если объект на поверхности, проверяем, не выходит ли он за ее границы
        if (parentObj && (parentObj->obType == objectType::Table || parentObj->obType == objectType::Cabinet))
        {
            QRectF surfaceRect = parentObj->boundingRect().translated(parentObj->pos());
            QRectF newRect(newPos, QSizeF(width, height));

            if (!surfaceRect.contains(newRect))
            {
                if (!surfaceRect.contains(newRect))
                {
                    // Пытаемся скорректировать позицию
                    if (newRect.left() < surfaceRect.left())
                        newPos.setX(surfaceRect.left());
                    else if (newRect.right() > surfaceRect.right())
                        newPos.setX(surfaceRect.right() - width);

                    if (newRect.top() < surfaceRect.top())
                        newPos.setY(surfaceRect.top());
                    else if (newRect.bottom() > surfaceRect.bottom())
                        newPos.setY(surfaceRect.bottom() - height);

                    newRect.moveTo(newPos);
                    if (!surfaceRect.contains(newRect))
                    {
                        // Если не помещается, отменяем поворот
                        width = oldWidth;
                        height = oldHeight;
                        angle = (angle - step) % 360;
                        if (angle < 0) angle += 360;
                        update();
                        event->ignore();
                        return;
                    }
                }
            }
        }

        setPos(newPos);
    }
    else
    {
        // Для квадратных объектов просто устанавливаем угол поворота
        setRotation(angle);
    }

    update();
    event->accept();
}

// Проверка, является ли объект мебелью
bool RoomObject::isFurniture() const
{
    return obType == objectType::Armchair ||
           obType == objectType::Chair ||
           obType == objectType::Table ||
           obType == objectType::Cabinet;
}

// Проверка, является ли объект сетевым устройством
bool RoomObject::isNetworkDevice() const
{
    return obType == objectType::Server ||
           obType == objectType::Computer ||
           obType == objectType::Printer ||
           obType == objectType::IPPhone ||
           obType == objectType::Router ||
           obType == objectType::IPCamera;
}

// Оператор объединения типов областей
RoomObject::areaType operator|(RoomObject::areaType a, RoomObject::areaType b)
{
    return static_cast<RoomObject::areaType>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

// Прикрепление объекта к родителю
void RoomObject::attach(RoomObject* parent)
{
    // Удаляем себя из списка дочерних объектов старого родителя
    if (parentObj)
    {
        parentObj->childObjs.removeOne(this);
    }

    parentObj = parent;
    if (parent)
    {
        // Добавляем себя к новому родителю
        parent->childObjs.append(this);

        // Проверяем пересечение с другими устройствами на той же поверхности
        for (RoomObject* sibling : parent->childObjs)
        {
            if (sibling != this && sibling->isNetworkDevice())
            {
                if (boundingRect().translated(pos()).intersects(
                        sibling->boundingRect().translated(sibling->pos())))
                {
                    // Если есть пересечение, возвращаемся на предыдущую позицию
                    setPos(validPos);
                    QMessageBox::warning(nullptr, "Ошибка размещения",
                                         "Устройство не может пересекаться с другими устройствами на поверхности");
                    return;
                }
            }
        }

        // Обновляем валидную позицию относительно родителя
        QPointF relativePos = pos() - parentObj->pos();
        validPos = parentObj->pos() + relativePos;
    }
}

// Обработчик входа курсора в область объекта
void RoomObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    QToolTip::showText(QCursor::pos(), this->objName);
    QGraphicsObject::hoverEnterEvent(event);
}

// Обработчик выхода курсора из области объекта
void RoomObject::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    QToolTip::hideText();
    QGraphicsObject::hoverLeaveEvent(event);
}

// Преобразование типа объекта в строку
QString RoomObject::typeToString(objectType type)
{
    switch (type)
    {
    case objectType::Wall: return "wall";
    case objectType::Door: return "door";
    case objectType::Window: return "window";
    case objectType::Computer: return "computer";
    case objectType::Table: return "table";
    case objectType::Cabinet: return "cabinet";
    case objectType::Chair: return "chair";
    case objectType::Armchair: return "armchair";
    case objectType::Server: return "server";
    case objectType::Printer: return "printer";
    case objectType::IPPhone: return "ip_phone";
    case objectType::Router: return "router";
    case objectType::IPCamera: return "ip_camera";
    default: return "unknown";
    }
}

// Преобразование строки в тип объекта
RoomObject::objectType RoomObject::stringToType(const QString& str)
{
    if (str == "wall") return objectType::Wall;
    if (str == "door") return objectType::Door;
    if (str == "window") return objectType::Window;
    if (str == "computer") return objectType::Computer;
    if (str == "table") return objectType::Table;
    if (str == "cabinet") return objectType::Cabinet;
    if (str == "chair") return objectType::Chair;
    if (str == "armchair") return objectType::Armchair;
    if (str == "server") return objectType::Server;
    if (str == "printer") return objectType::Printer;
    if (str == "ip_phone") return objectType::IPPhone;
    if (str == "router") return objectType::Router;
    if (str == "ip_camera") return objectType::IPCamera;
}

