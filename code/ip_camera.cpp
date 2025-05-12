#include "ip_camera.h"

// Конструктор
IPCamera::IPCamera(QGraphicsObject *parent) : NetworkDevice(parent, RoomObject::objectType::IPCamera)
{
    this->objName = "IP-камера";  // Установка имени объекта
    this->obType = objectType::IPCamera;  // Установка типа объекта
    this->arType = OnFloor | NearWall;  // Установка типа размещения (на полу или у стены)

    // Получение общего изображения и установка размеров объекта
    QPixmap& pixmap = getCommonPixmap();
    setSize(pixmap.width(), pixmap.height());
}

// Деструктор
IPCamera::~IPCamera()
{

}

// Метод для клонирования объекта
IPCamera* IPCamera::clone() const
{
    // Создание нового объекта
    IPCamera* newIPCamera = new IPCamera(nullptr);
    // Копирование свойств текущего объекта в новый
    newIPCamera->objName = this->objName;
    newIPCamera->obType = this->obType;
    newIPCamera->arType = this->arType;
    newIPCamera->cType = this->cType;
    newIPCamera->setFeatures(this->devFeatures);
    newIPCamera->setZValue(this->zValue());
    newIPCamera->setPos(this->pos());

    // Установка размеров нового объекта
    QPixmap& pixmap = newIPCamera->getCommonPixmap();
    newIPCamera->setSize(pixmap.width(), pixmap.height());

    return newIPCamera;
}

// Возвращает изображение объекта
QPixmap IPCamera::objectPixmap() const
{
    return getCommonPixmap();
}

// Отрисовывает объект на painter
void IPCamera::drawObject(QPainter* painter) const
{
    painter->drawPixmap(0, 0, getCommonPixmap());
}

// Статический метод для получения общего изображения IP-камеры
QPixmap& IPCamera::getCommonPixmap()
{
    static QPixmap commonPixmap;  // Статическое изображение для всех экземпляров
    if (commonPixmap.isNull())  // Если изображение еще не загружено
    {
        // Попытка загрузки изображения из ресурсов
        commonPixmap.load(":/images/camera.png");
        if (commonPixmap.isNull())  // Если загрузка не удалась
        {
            qWarning("Не удалось загрузить изображение");
            // Создание временного изображения
            commonPixmap = QPixmap(35, 35);
            commonPixmap.fill(Qt::darkRed);
        }
        else
        {
            commonPixmap = commonPixmap.scaled(35, 35, Qt::KeepAspectRatio);
        }
    }
    return commonPixmap;
}

// Возвращает полигон объекта в координатах сцены
QPolygonF IPCamera::scenePolygon() const
{
    QRectF rect = boundingRect();  // Получение ограничивающего прямоугольника
    // Преобразование углов прямоугольника в координаты сцены и создание полигона
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
