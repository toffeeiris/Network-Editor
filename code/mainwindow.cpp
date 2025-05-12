#include "mainwindow.h"
#include "ui_mainwindow.h"

// Заголовочные файлы для объектов комнаты
#include "cabinet.h"
#include "chair.h"
#include "armchair.h"
#include "table.h"
#include "computer.h"
#include "server.h"
#include "printer.h"
#include "ip_camera.h"
#include "ip_phone.h"
#include "router.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Установка иконок для кнопок
    updateIcon(ui->load, ":/images/load.png", "Загрузить план");
    updateIcon(ui->save, ":/images/save.png", "Сохранить");
    updateIcon(ui->inf, ":/images/inf.png", "Информация об управлении");
    updateIcon(ui->exit, ":/images/exit.png", "Выход");

    // Центрирование окна на экране
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    // Инициализация графической сцены
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, ui->plan_of_room->width(), ui->plan_of_room->height());

    // Установка фонового изображения
    QPixmap temp_bg(":/main/picture.jpg");
    QGraphicsPixmapItem *bg_item = scene->addPixmap(temp_bg);

    // Масштабирование фона
    qreal scale_x = ui->plan_of_room->width() / qreal(temp_bg.width());
    qreal scale_y = ui->plan_of_room->height() / qreal(temp_bg.height());
    qreal scale_factor = qMin(scale_x, scale_y);
    bg_item->setScale(scale_factor);

    // Центрирование фона
    qreal center_x = (ui->plan_of_room->width() - temp_bg.width() * scale_factor) / 2;
    qreal center_y = (ui->plan_of_room->height() - temp_bg.height() * scale_factor) / 2;
    bg_item->setPos(center_x, center_y);

    // Настройка отображения сцены
    ui->plan_of_room->setScene(scene);
    ui->plan_of_room->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->plan_of_room->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->plan_of_room->setDragMode(QGraphicsView::NoDrag);
    ui->plan_of_room->setInteractive(true);
    ui->plan_of_room->setRenderHint(QPainter::Antialiasing);

    // Инициализация панели сетевых устройств
    QGraphicsScene *sceneDev = new QGraphicsScene(this);
    QGraphicsView *viewDev = new QGraphicsView(sceneDev);
    ui->scrollDev->setWidget(viewDev);
    ui->scrollDev->setWidgetResizable(true);
    ui->scrollDev->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewDev->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewDev->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewDev->setDragMode(QGraphicsView::NoDrag);

    // Создание и размещение сетевых устройств
    QVector<RoomObject*> netObjects = {new Router(), new IPPhone(), new IPCamera(),
                                        new Printer(), new Computer(), new Server()};
    int scrollWDev = ui->scrollDev->width() - 20;
    int itemH = 40;
    int space = 20;

    for (int i = 0; i < netObjects.size(); i++)
    {
        RoomObject* curr = netObjects[i];
        curr->setContainerType(RoomObject::containerType::ScrollArea);
        sceneDev->addItem(curr);

        // Центрирование объектов по горизонтали
        int xPos = (scrollWDev - curr->boundingRect().width()) / 2;
        int yPos = i * (itemH + space);
        curr->setPos(xPos, yPos);

        // Подключение сигналов
        connect(curr, &RoomObject::dragObject, this, &MainWindow::handleDragObject);
        connect(curr, &RoomObject::droppedObject, this, &MainWindow::handleDroppedObject);
        connect(curr, &RoomObject::deleteObject, this, &MainWindow::handleDeleteObject);
    }

    sceneDev->setSceneRect(0, -10, scrollWDev, netObjects.size() * (itemH + space));

    // Инициализация панели мебели (аналогично панели устройств)
    QGraphicsScene *sceneFurn = new QGraphicsScene(this);
    QGraphicsView *viewFurn = new QGraphicsView(sceneFurn);
    ui->scrollFurn->setWidget(viewFurn);
    ui->scrollFurn->setWidgetResizable(true);
    ui->scrollFurn->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewFurn->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewFurn->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewFurn->setDragMode(QGraphicsView::NoDrag);

    QVector<RoomObject*> furnObjects = {new Armchair(), new Cabinet(), new Chair(), new Table()};
    int scrollWFurn = ui->scrollFurn->width() - 20;
    int totalH = 0;

    for (int i = 0; i < furnObjects.size(); i++)
    {
        RoomObject* curr = furnObjects[i];
        curr->setContainerType(RoomObject::containerType::ScrollArea);
        sceneFurn->addItem(curr);

        QRectF rect = curr->boundingRect();
        int objectW = rect.width();
        int objectH = rect.height();

        int xPos = (scrollWFurn - objectW) / 2;
        int yPos = totalH;
        totalH += objectH + space;
        curr->setPos(xPos, yPos);

        connect(curr, &RoomObject::dragObject, this, &MainWindow::handleDragObject);
        connect(curr, &RoomObject::droppedObject, this, &MainWindow::handleDroppedObject);
        connect(curr, &RoomObject::deleteObject, this, &MainWindow::handleDeleteObject);
    }

    sceneFurn->setSceneRect(0, -10, scrollWFurn, totalH);
}

// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}

// Обработка перетаскивания объекта
void MainWindow::handleDragObject(RoomObject* source, const QPointF& scenePos)
{
    if(!isPlanLoaded)
    {
        QMessageBox::warning(nullptr, "Ошибка", "Сначала загрузите план комнаты");
        return;
    }

    if (!currDrag)
    {
        // Создаем клон объекта для перетаскивания
        currDrag = source->clone();
        QPointF cursorPos = ui->plan_of_room->mapToScene(ui->plan_of_room->mapFromGlobal(QCursor::pos()));
        currDrag->setPos(cursorPos);
        ui->plan_of_room->scene()->addItem(currDrag);
    }
    else
    {
        // Обновляем позицию перетаскиваемого объекта
        QPointF cursorPos = ui->plan_of_room->mapToScene(ui->plan_of_room->mapFromGlobal(QCursor::pos()));
        currDrag->setPos(cursorPos);
    }
}

// Обработка отпускания объекта
void MainWindow::handleDroppedObject(RoomObject* source, const QPointF& scenePos)
{
    if (currDrag)
    {
        QPointF viewPos = ui->plan_of_room->mapFromGlobal(QCursor::pos());
        QPointF dropPos = ui->plan_of_room->mapToScene(viewPos.toPoint());

        // Проверка возможности размещения объекта
        if (!currDrag->canBePlaced(dropPos))
        {
            ui->plan_of_room->scene()->removeItem(currDrag);
            delete currDrag;
            currDrag = nullptr;

            // Сообщение об ошибке в зависимости от типа объекта
            QString currWarn;
            switch(source->getObjectType())
            {
            case RoomObject::objectType::Server:
                currWarn = "Сервер можно разместить только на полу";
                break;
            case RoomObject::objectType::Computer:
                currWarn = "Компьютер можно разместить только на столе";
                break;
            case RoomObject::objectType::Printer:
                currWarn = "Принтер можно разместить только на столе или на тумбе";
                break;
            case RoomObject::objectType::IPPhone:
                currWarn = "IP-телефон можно разместить только на столе или на тумбе";
                break;
            case RoomObject::objectType::Router:
                currWarn = "Роутер можно разместить только на столе или на тумбе";
                break;
            case RoomObject::objectType::IPCamera:
                currWarn = "IP-камеру можно разместить только возле стены";
                break;
            default:
                currWarn = "Некорректное размещение";
                break;
            }

            QMessageBox::warning(nullptr, "Ошибка размещения", currWarn);
            return;
        }

        // Фиксация объекта на сцене
        currDrag->setContainerType(RoomObject::containerType::MainScene);
        currDrag->setFlag(QGraphicsItem::ItemIsMovable, true);
        currDrag->setFlag(QGraphicsItem::ItemIsSelectable, true);
        currDrag->setPos(dropPos);

        connect(currDrag, &RoomObject::deleteObject, this, &MainWindow::handleDeleteObject);
        currDrag = nullptr;
    }
}

// Удаление объекта
void MainWindow::handleDeleteObject(RoomObject* source)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Удалить элемент", "Вы уверены, что хотите удалить?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes && source)
    {
        if (ui->plan_of_room->scene()->items().contains(source))
        {
            disconnect(source, &RoomObject::deleteObject, this, &MainWindow::handleDeleteObject);
            ui->plan_of_room->scene()->removeItem(source);
            delete source;
        }
    }
}

// Установка иконки кнопки
void MainWindow::updateIcon(QPushButton *button, const QString& path, const QString& tip)
{
    QPixmap pixmap(path);
    if (!pixmap.isNull())
    {
        button->setIcon(QIcon(pixmap.scaled(button->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        button->setIconSize(button->size());
        button->setToolTip(tip);
    }
}

// Загрузка плана из XML
void MainWindow::on_load_pressed()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Загрузить план помещения", "", "XML Files (*.xml)");
    if (fileName.isEmpty())
        return;

    scene->clear();
    ui->plan_of_room->centerOn(0, 0);

    QString currAnswer = parser.loadXML(fileName, ui->plan_of_room->scene());
    if (currAnswer == "ОК")
    {
        isPlanLoaded = true;
        // Добавление сетки как фоновой текстуры
        QPixmap bg(15, 15);
        bg.fill(Qt::transparent);

        QPainter painter(&bg);
        painter.setPen(QColor(200, 200, 200, 120)); // Светло-серая полупрозрачная сетка
        painter.drawLine(0, 0, 0, 20); // вертикальная линия
        painter.drawLine(0, 0, 20, 0); // горизонтальная линия
        painter.end();

        ui->plan_of_room->setBackgroundBrush(QBrush(bg));

        // Настройка загруженных объектов
        for (QGraphicsItem* item : scene->items())
        {
            if (RoomObject* obj = dynamic_cast<RoomObject*>(item))
            {
                obj->setContainerType(RoomObject::containerType::MainScene);
                obj->setFlag(QGraphicsItem::ItemIsMovable, true);
                obj->setFlag(QGraphicsItem::ItemIsSelectable, true);
                connect(obj, &RoomObject::deleteObject, this, &MainWindow::handleDeleteObject);
            }
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "Ошибка", currAnswer);
    }
}

// Сохранение плана в XML
void MainWindow::on_save_pressed()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Сохранить файл", "", "XML Files (*.xml)");
    if (fileName.isEmpty())
        return;

    Parser parser;
    QList<QGraphicsItem*> items = scene->items();

    if (!parser.saveXML(fileName, items))
    {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось сохранить файл.");
    } else
    {
        QMessageBox::information(nullptr, "Сохранение", "Файл успешно сохранён.");
    }
}

// Показ информации об управлении
void MainWindow::on_inf_pressed()
{
    QString currInf = "Левая кнопка мыши - перемещение\n"
                      "Левая кнопка мыши + вращение колесика - поворот предмета (90 градусов)\n"
                      "Правая кнопка мыши - удаление предмета\n"
                      "Двоной клик левой кнопкой мыши - задание параметров сетевого устройства\n"
                      "Замечание: чтобы сетевое устройство прикрепилось к столу/тумбе нужно, поставив объект, немного его переместить\n";
    QMessageBox::information(nullptr, "Управление", currInf);
}

// Выход из приложения
void MainWindow::on_exit_pressed()
{
    exit(0);
}


