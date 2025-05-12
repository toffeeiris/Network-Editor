#ifndef ROOM_OBJECT_H
#define ROOM_OBJECT_H

#include <QObject>
#include <QGraphicsObject>
#include <QPainter>
#include <QPixmap>
#include <QMessageBox>
#include <QToolTip>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>

using namespace std;

class RoomObject : public QGraphicsObject
{
    Q_OBJECT;


public:

    // Типы объектов
    enum class objectType {
        Wall, Door, Window, FreeSpace, \
        Table, Cabinet, Chair, Armchair, \
        Server, Computer, Printer, IPPhone, Router, IPCamera
    };

    // Типы контейнеров для объектов
    enum class containerType {
        ScrollArea,  // В области прокрутки (панель инструментов)
        MainScene    // На основной сцене
    };

    // Типы областей размещения объектов
    enum areaType {
        OnTable = 1 << 0,   // На столе
        OnCabinet = 1 << 1, // На шкафу
        OnFloor = 1 << 2,   // На полу
        NearWall = 1 << 3   // У стены
    };

    Q_DECLARE_FLAGS(AllowedAreas, areaType)

    // Конструкторы
    explicit RoomObject(QGraphicsObject *parent = nullptr);
    explicit RoomObject(objectType type, QGraphicsObject* parent = nullptr);
    virtual ~RoomObject();

    // Методы QGraphicsObject
    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    // Установка размеров объекта
    void setSize(int, int);

    // Установка типа контейнера
    void setContainerType(containerType currType) {
        cType = currType;
        setFlag(ItemIsMovable, currType == containerType::MainScene);
    }

    // Установка типа объекта
    void setObjectType(objectType currType) { this->obType = currType; }

    // Установка типа области размещения
    void setAreaType(areaType currType) { arType = currType; }

    // Получение типа объекта
    objectType getObjectType() const { return obType; }

    // Получение типа области размещения
    areaType getAreaType() const { return arType; }

    // Виртуальный метод для клонирования объекта
    virtual RoomObject* clone() const = 0;

    // Проверка возможности размещения объекта в позиции
    bool canBePlaced(const QPointF& pos) const;

    // Проверка типа объекта
    bool isFurniture() const;
    bool isNetworkDevice() const;

    // Оператор для комбинации типов областей
    friend areaType operator|(areaType a, areaType b);

    // Преобразование между типом объекта и строкой
    static QString typeToString(objectType type);
    static objectType stringToType(const QString& str);

    // Получение полигона объекта
    virtual QPolygonF scenePolygon() const;
    virtual QPolygonF getPolygon() const;

protected:

    // Виртуальные методы для отрисовки объекта
    virtual QPixmap objectPixmap() const = 0;
    virtual void drawObject(QPainter* painter) const;

    // Обработчики событий мыши
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    // Обработчики событий наведения
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    // Прикрепление объекта к родителю
    void attach(RoomObject* parent);

    // Обработчик события колесика мыши
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;



signals:
    // Сигналы для работы с объектом
    void dragObject(RoomObject* source, const QPointF& scenePos);
    void droppedObject(RoomObject* source, const QPointF& scenePos);
    void deleteObject(RoomObject* source);

protected:
    // Свойства объекта
    QString objName;            // Название объекта
    objectType obType;          // Тип объекта
    areaType arType;            // Тип области размещения
    containerType cType = containerType::MainScene; // Тип контейнера

    int width = 0, height = 0;  // Размеры объекта
    int angle = 0;              // Угол поворота

private:
    // Позиции для перемещения
    QPointF dragStartPos;       // Начальная позиция при перетаскивании
    QPointF validPos;           // Последняя валидная позиция
    bool isDragging = false;    // Флаг перетаскивания

    RoomObject* parentObj = nullptr;     // Родительский объект
    QList<RoomObject*> childObjs;        // Дочерние объекты

};

#endif // ROOM_OBJECT_H
