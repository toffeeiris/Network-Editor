#include "server.h"

// Конструктор
Server::Server(QGraphicsObject *parent) : NetworkDevice(parent, RoomObject::objectType::Server)
{
    this->objName = "Сервер";  // Установка имени объекта
    this->obType = objectType::Server;  // Установка типа объекта
    this->arType = OnFloor;  // Установка типа размещения (на полу)

    // Получение общего изображения и установка размеров объекта
    QPixmap& pixmap = getCommonPixmap();
    setSize(pixmap.width(), pixmap.height());
}

// Деструктор
Server::~Server()
{

}

// Метод для клонирования объекта
Server* Server::clone() const
{
    // Создание нового объекта
    Server* newServer = new Server(nullptr);
    // Копирование свойств текущего объекта в новый
    newServer->objName = this->objName;
    newServer->obType = this->obType;
    newServer->arType = this->arType;
    newServer->cType = this->cType;
    newServer->setFeatures(this->devFeatures);
    newServer->setZValue(this->zValue());
    newServer->setPos(this->pos());

    // Установка размеров нового объекта
    QPixmap& pixmap = newServer->getCommonPixmap();
    newServer->setSize(pixmap.width(), pixmap.height());

    return newServer;
}

// Возвращает изображение объекта
QPixmap Server::objectPixmap() const
{
    return getCommonPixmap();
}

// Отрисовывает объект на painter
void Server::drawObject(QPainter* painter) const
{
    painter->drawPixmap(0, 0, getCommonPixmap());
}

// Статический метод для получения общего изображения сервера
QPixmap& Server::getCommonPixmap()
{
    static QPixmap commonPixmap;  // Статическое изображение для всех экземпляров
    if (commonPixmap.isNull())  // Если изображение еще не загружено
    {
        // Попытка загрузки изображения из ресурсов
        commonPixmap.load(":/images/server.png");
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
QPolygonF Server::scenePolygon() const
{
    QRectF rect = boundingRect();  // Получение ограничивающего прямоугольника
    // Преобразование углов прямоугольника в координаты сцены и создание полигона
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
