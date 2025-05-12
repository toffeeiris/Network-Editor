#include "cabinet.h"

// Конструктор
Cabinet::Cabinet(QGraphicsObject *parent) : Furniture(parent, RoomObject::objectType::Table)
{
    this->objName = "Тумба"; // Установка названия объекта
    this->obType = objectType::Cabinet; // Установка типа объекта
    setSize(55, 55); // Установка размеров тумбы (55x55 пикселей)
}

// Деструктор
Cabinet::~Cabinet()
{

}

// Создание копии объекта тумбы
Cabinet* Cabinet::clone() const
{
    Cabinet* newCabinet = new Cabinet(nullptr); // Создание нового объекта

    // Копирование всех свойств текущего объекта
    newCabinet->objName = this->objName;
    newCabinet->obType = this->obType;
    newCabinet->arType = this->arType;
    newCabinet->cType = this->cType;
    newCabinet->setZValue(this->zValue()); // Копирование Z-координаты
    newCabinet->setPos(this->pos()); // Копирование позиции

    // Копирование размеров и цветов
    newCabinet->setSize(width, height);
    newCabinet->color = this->color;
    newCabinet->border = this->border;

    return newCabinet;
}

// Генерация изображения тумбы
QPixmap Cabinet::objectPixmap() const
{
    QPixmap pixmap(width, height); // Создание pixmap нужного размера
    pixmap.fill(Qt::transparent); // Прозрачный фон

    QPainter painter(&pixmap); // Создание painter для рисования
    drawObject(&painter); // Отрисовка тумбы

    return pixmap;
}

// Отрисовка тумбы
void Cabinet::drawObject(QPainter* painter) const
{
    painter->setRenderHint(QPainter::Antialiasing, false); // Отключение сглаживания
    painter->setPen(QPen(border, 3)); // Установка пера (толщина 3 пикселя)
    painter->setBrush(color); // Установка кисти (основной цвет)

    // Рисование прямоугольника с учетом толщины границы
    painter->drawRect(3/2, 3/2, width - 3, height - 3);
}

// Получение полигона тумбы в координатах сцены
QPolygonF Cabinet::scenePolygon() const
{
    QRectF rect = boundingRect(); // Получение ограничивающего прямоугольника

    // Преобразование углов прямоугольника в координаты сцены
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
