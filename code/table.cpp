#include "table.h"

// Конструктор
Table::Table(QGraphicsObject *parent) : Furniture(parent, RoomObject::objectType::Table)
{
    this->objName = "Стол"; // Установка названия объекта
    this->obType = objectType::Table; // Установка типа объекта
    setSize(60, 130); // Установка размеров стола (60x130 пикселей)
}

// Деструктор
Table::~Table()
{

}

// Создание копии объекта стола
Table* Table::clone() const
{
    Table* newTable = new Table(nullptr); // Создание нового объекта

    // Копирование всех свойств текущего стола
    newTable->objName = this->objName;
    newTable->obType = this->obType;
    newTable->arType = this->arType;
    newTable->cType = this->cType;
    newTable->setZValue(this->zValue()); // Копирование Z-координаты
    newTable->setPos(this->pos()); // Копирование позиции

    // Копирование размеров и цветовых параметров
    newTable->setSize(width, height);
    newTable->color = this->color;
    newTable->border = this->border;

    return newTable;
}

// Генерация изображения стола
QPixmap Table::objectPixmap() const
{
    QPixmap pixmap(width, height); // Создание изображения нужного размера
    pixmap.fill(Qt::transparent); // Прозрачный фон

    QPainter painter(&pixmap); // Создание объекта для рисования
    drawObject(&painter); // Отрисовка стола

    return pixmap;
}

// Метод отрисовки стола
void Table::drawObject(QPainter* painter) const
{
    painter->setRenderHint(QPainter::Antialiasing, false); // Отключение сглаживания
    painter->setPen(QPen(border, 3)); // Установка пера (толщина 3 пикселя)
    painter->setBrush(color); // Установка цвета заливки

    // Рисование прямоугольника с учетом толщины границы
    // Смещение на 1.5 пикселя (3/2) и уменьшение размера на 3 пикселя
    painter->drawRect(3/2, 3/2, width - 3, height - 3);
}

// Получение полигона стола в координатах сцены
QPolygonF Table::scenePolygon() const
{
    QRectF rect = boundingRect(); // Получение ограничивающего прямоугольника

    // Преобразование углов прямоугольника в координаты сцены
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
