#include "ip_phone.h"

// Конструктор
IPPhone::IPPhone(QGraphicsObject *parent) : NetworkDevice(parent, RoomObject::objectType::IPPhone)
{
    this->objName = "IP-телефон";  // Установка имени объекта
    this->obType = objectType::IPPhone;  // Установка типа объекта
    this->arType = OnTable | OnCabinet;  // Установка типа размещения (на столе или шкафу)

    // Получение общего изображения и установка размеров объекта
    QPixmap& pixmap = getCommonPixmap();
    setSize(pixmap.width(), pixmap.height());
}

// Деструктор
IPPhone::~IPPhone()
{

}

// Метод для клонирования объекта
IPPhone* IPPhone::clone() const
{
    // Создание нового объекта
    IPPhone* newIPPhone = new IPPhone(nullptr);
    // Копирование свойств текущего объекта в новый
    newIPPhone->objName = this->objName;
    newIPPhone->obType = this->obType;
    newIPPhone->arType = this->arType;
    newIPPhone->cType = this->cType;
    newIPPhone->setFeatures(this->devFeatures);
    newIPPhone->setZValue(this->zValue());
    newIPPhone->setPos(this->pos());

    // Установка размеров нового объекта
    QPixmap& pixmap = newIPPhone->getCommonPixmap();
    newIPPhone->setSize(pixmap.width(), pixmap.height());

    return newIPPhone;
}

// Возвращает изображение объекта
QPixmap IPPhone::objectPixmap() const
{
    return getCommonPixmap();
}

// Отрисовывает объект на painter
void IPPhone::drawObject(QPainter* painter) const
{
    painter->drawPixmap(0, 0, getCommonPixmap());
}

// Статический метод для получения общего изображения IP-телефона
QPixmap& IPPhone::getCommonPixmap()
{
    static QPixmap commonPixmap;  // Статическое изображение для всех экземпляров
    if (commonPixmap.isNull())  // Если изображение еще не загружено
    {
        // Попытка загрузки изображения из ресурсов
        commonPixmap.load(":/images/phone.png");
        if (commonPixmap.isNull())  // Если загрузка не удалась
        {
            qWarning("Не удалось загрузить изображение");
            // Создание временного изображения
            commonPixmap = QPixmap(45, 45);
            commonPixmap.fill(Qt::darkRed);
        }
        else
        {
            commonPixmap = commonPixmap.scaled(45, 45, Qt::KeepAspectRatio);
        }
    }
    return commonPixmap;
}

// Возвращает полигон объекта в координатах сцены
QPolygonF IPPhone::scenePolygon() const
{
    QRectF rect = boundingRect();  // Получение ограничивающего прямоугольника
    // Преобразование углов прямоугольника в координаты сцены и создание полигона
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
