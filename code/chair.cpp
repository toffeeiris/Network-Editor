#include "chair.h"

// Конструктор
Chair::Chair(QGraphicsObject *parent) : Furniture(parent, RoomObject::objectType::Table)
{
    this->objName = "Стул"; // Установка названия объекта
    this->obType = objectType::Chair; // Установка типа объекта
    setSize(45, 45); // Установка размеров стула (45x45 пикселей)
}

// Деструктор
Chair::~Chair()
{

}

// Создание копии объекта стула
Chair* Chair::clone() const
{
    Chair* newChair = new Chair(nullptr); // Создание нового объекта

    // Копирование всех свойств текущего стула
    newChair->objName = this->objName;
    newChair->obType = this->obType;
    newChair->arType = this->arType;
    newChair->cType = this->cType;
    newChair->setZValue(this->zValue()); // Копирование Z-координаты
    newChair->setPos(this->pos()); // Копирование позиции

    // Копирование размеров и цветовых параметров
    newChair->setSize(width, height);
    newChair->color = this->color;
    newChair->border = this->border;

    return newChair;
}

// Генерация изображения стула
QPixmap Chair::objectPixmap() const
{
    QPixmap pixmap(width, height); // Создание изображения нужного размера
    pixmap.fill(Qt::transparent); // Прозрачный фон

    QPainter painter(&pixmap); // Создание объекта для рисования
    drawObject(&painter); // Отрисовка стула

    return pixmap;
}

// Метод отрисовки стула
void Chair::drawObject(QPainter* painter) const
{
    painter->setRenderHint(QPainter::Antialiasing, true); // Включение сглаживания
    painter->setPen(QPen(border, 3)); // Установка пера (толщина 3 пикселя)
    painter->setBrush(color); // Установка цвета заливки

    // Рисование эллипса (круглой формы стула)
    // Смещение на 1 пиксель и уменьшение размера на 2 пикселя для учета толщины границы
    painter->drawEllipse(1, 1, width-2, height-2);
}

// Получение полигона стула в координатах сцены
QPolygonF Chair::scenePolygon() const
{
    QRectF rect = boundingRect(); // Получение ограничивающего прямоугольника

    // Преобразование углов прямоугольника в координаты сцены
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
