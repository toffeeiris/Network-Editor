#include "computer.h"

// Конструктор
Computer::Computer(QGraphicsObject *parent) : NetworkDevice(parent, RoomObject::objectType::Computer)
{
    this->objName = "Компьютер";  // Установка имени объекта
    this->obType = objectType::Computer;  // Установка типа объекта
    this->arType = OnTable;  // Установка типа размещения (на столе)

    // Получение общего изображения и установка размеров объекта
    QPixmap& pixmap = getCommonPixmap();
    setSize(pixmap.width(), pixmap.height());
}

// Деструктор
Computer::~Computer()
{

}

// Метод для клонирования объекта
Computer* Computer::clone() const
{
    // Создание нового объекта
    Computer* newComputer = new Computer(nullptr);
    // Копирование свойств текущего объекта в новый
    newComputer->objName = this->objName;
    newComputer->obType = this->obType;
    newComputer->arType = this->arType;
    newComputer->cType = this->cType;
    newComputer->setFeatures(this->devFeatures);
    newComputer->setZValue(this->zValue());
    newComputer->setPos(this->pos());

    // Установка размеров нового объекта
    QPixmap& pixmap = newComputer->getCommonPixmap();
    newComputer->setSize(pixmap.width(), pixmap.height());

    return newComputer;
}

// Возвращает изображение объекта
QPixmap Computer::objectPixmap() const
{
    return getCommonPixmap();
}

// Отрисовывает объект на painter
void Computer::drawObject(QPainter* painter) const
{
    painter->drawPixmap(0, 0, getCommonPixmap());
}

// Статический метод для получения общего изображения компьютера
QPixmap& Computer::getCommonPixmap()
{
    static QPixmap commonPixmap;  // Статическое изображение для всех экземпляров
    if (commonPixmap.isNull())  // Если изображение еще не загружено
    {
        // Попытка загрузки изображения из ресурсов
        commonPixmap.load(":/images/computer.png");
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
QPolygonF Computer::scenePolygon() const
{
    QRectF rect = boundingRect();  // Получение ограничивающего прямоугольника
    // Преобразование углов прямоугольника в координаты сцены и создание полигона
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
