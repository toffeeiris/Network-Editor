#include "printer.h"

// Конструктор
Printer::Printer(QGraphicsObject *parent) : NetworkDevice(parent, RoomObject::objectType::Printer)
{
    this->objName = "Принтер";  // Установка имени объекта
    this->obType = objectType::Printer;  // Установка типа объекта
    this->arType = OnTable | OnCabinet;  // Установка типа размещения (на столе или шкафу)

    // Получение общего изображения и установка размеров объекта
    QPixmap& pixmap = getCommonPixmap();
    setSize(pixmap.width(), pixmap.height());
}

// Деструктор
Printer::~Printer()
{

}

// Метод для клонирования объекта
Printer* Printer::clone() const
{
    // Создание нового объекта
    Printer* newPrinter = new Printer(nullptr);
    // Копирование свойств текущего объекта в новый
    newPrinter->objName = this->objName;
    newPrinter->obType = this->obType;
    newPrinter->arType = this->arType;
    newPrinter->cType = this->cType;
    newPrinter->setFeatures(this->devFeatures);
    newPrinter->setZValue(this->zValue());
    newPrinter->setPos(this->pos());

    // Установка размеров нового объекта
    QPixmap& pixmap = newPrinter->getCommonPixmap();
    newPrinter->setSize(pixmap.width(), pixmap.height());

    return newPrinter;
}

// Возвращает изображение объекта
QPixmap Printer::objectPixmap() const
{
    return getCommonPixmap();
}

// Отрисовывает объект на painter
void Printer::drawObject(QPainter* painter) const
{
    painter->drawPixmap(0, 0, getCommonPixmap());
}

// Статический метод для получения общего изображения принтера
QPixmap& Printer::getCommonPixmap()
{
    static QPixmap commonPixmap;  // Статическое изображение для всех экземпляров
    if (commonPixmap.isNull())  // Если изображение еще не загружено
    {
        // Попытка загрузки изображения из ресурсов
        commonPixmap.load(":/images/printer.png");
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
QPolygonF Printer::scenePolygon() const
{
    QRectF rect = boundingRect();  // Получение ограничивающего прямоугольника
    // Преобразование углов прямоугольника в координаты сцены и создание полигона
    return QPolygonF() << mapToScene(rect.topLeft())
                       << mapToScene(rect.topRight())
                       << mapToScene(rect.bottomRight())
                       << mapToScene(rect.bottomLeft());
}
