#include "router.h"

// Конструктор
Router::Router(QGraphicsObject *parent) : NetworkDevice(parent, RoomObject::objectType::Router)
{
    this->objName = "Маршрутизатор";  // Установка имени объекта
    this->obType = objectType::Router;  // Установка типа объекта
    this->arType = OnTable | OnCabinet;  // Установка типа размещения (на столе или шкафу)

    // Получение общего изображения и установка размеров объекта
    QPixmap& pixmap = getCommonPixmap();
    setSize(pixmap.width(), pixmap.height());
}

// Деструктор
Router::~Router()
{

}

// Метод для клонирования объекта
Router* Router::clone() const
{
    // Создание нового объекта
    Router* newRouter = new Router(nullptr);
    // Копирование свойств текущего объекта в новый
    newRouter->objName = this->objName;
    newRouter->obType = this->obType;
    newRouter->arType = this->arType;
    newRouter->cType = this->cType;
    newRouter->setFeatures(this->devFeatures);
    newRouter->setZValue(this->zValue());
    newRouter->setPos(this->pos());

    // Установка размеров нового объекта
    QPixmap& pixmap = newRouter->getCommonPixmap();
    newRouter->setSize(pixmap.width(), pixmap.height());

    return newRouter;
}

// Возвращает изображение объекта
QPixmap Router::objectPixmap() const
{
    return getCommonPixmap();
}

// Отрисовывает объект на painter
void Router::drawObject(QPainter* painter) const
{
    painter->drawPixmap(0, 0, getCommonPixmap());
}

// Статический метод для получения общего изображения маршрутизатора
QPixmap& Router::getCommonPixmap()
{
    static QPixmap commonPixmap;  // Статическое изображение для всех экземпляров
    if (commonPixmap.isNull())  // Если изображение еще не загружено
    {
        // Попытка загрузки изображения из ресурсов
        commonPixmap.load(":/images/router.png");
        if (commonPixmap.isNull())  // Если загрузка не удалась
        {
            qWarning("Не удалось загрузить изображение");
            // Создание временного изображения
            commonPixmap = QPixmap(45, 45);
            commonPixmap.fill(Qt::darkRed);
        }
        else
        {
            // Масштабирование с сохранением пропорций
            commonPixmap = commonPixmap.scaled(45, 45, Qt::KeepAspectRatio);
        }
    }
    return commonPixmap;
}

// Возвращает полигон объекта в координатах сцены
QPolygonF Router::scenePolygon() const
{
    QRectF rect = boundingRect();  // Получение ограничивающего прямоугольника
    // Преобразование углов прямоугольника в координаты сцены и создание полигона
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
