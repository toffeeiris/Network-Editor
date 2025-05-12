#include "armchair.h"

// Конструктор
Armchair::Armchair(QGraphicsObject *parent) : Furniture(parent, RoomObject::objectType::Armchair)
{
    this->objName = "Кресло";  // Установка имени объекта
    this->obType = objectType::Armchair;  // Установка типа объекта

    // Получение и установка изображения кресла
    QPixmap& pixmap = getCommonPixmap();
    setSize(pixmap.width(), pixmap.height());  // Установка размеров объекта
}

// Деструктор
Armchair::~Armchair()
{

}

// Создание копии объекта кресла
Armchair* Armchair::clone() const
{
    // Создание нового объекта кресла
    Armchair* newArmchair = new Armchair(nullptr);

    // Копирование свойств текущего объекта
    newArmchair->objName = this->objName;
    newArmchair->obType = this->obType;
    newArmchair->arType = this->arType;
    newArmchair->cType = this->cType;
    newArmchair->setZValue(this->zValue());  // Копирование Z-координаты
    newArmchair->setPos(this->pos());  // Копирование позиции

    // Установка размеров на основе изображения
    QPixmap& pixmap = newArmchair->getCommonPixmap();
    newArmchair->setSize(pixmap.width(), pixmap.height());

    return newArmchair;
}

// Получение изображения кресла
QPixmap Armchair::objectPixmap() const
{
    return getCommonPixmap();
}

// Отрисовка кресла
void Armchair::drawObject(QPainter* painter) const
{
    painter->drawPixmap(0, 0, getCommonPixmap());
}

// Статический метод для получения общего изображения всех кресел
QPixmap& Armchair::getCommonPixmap()
{
    static QPixmap commonPixmap;  // Статическое изображение для всех экземпляров

    // Если изображение еще не загружено
    if (commonPixmap.isNull())
    {
        // Попытка загрузки изображения из ресурсов
        commonPixmap.load(":/images/armchair.png");

        // Если загрузка не удалась
        if (commonPixmap.isNull())
        {
            qWarning("Не удалось загрузить изображение");
            // Создание временного изображения
            commonPixmap = QPixmap(70, 60);
            commonPixmap.fill(Qt::darkRed);  // Заполнение темно-красным цветом
        }
        else
        {
            // Масштабирование загруженного изображения
            commonPixmap = commonPixmap.scaled(70, 60, Qt::KeepAspectRatio);
        }
    }
    return commonPixmap;
}

// Получение полигона кресла в координатах сцены
QPolygonF Armchair::scenePolygon() const
{
    QRectF rect = boundingRect();  // Получение ограничивающего прямоугольника
    // Преобразование углов прямоугольника в координаты сцены
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
