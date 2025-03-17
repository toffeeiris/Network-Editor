#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, ui->plan_of_room->width(), ui->plan_of_room->height());
    QPixmap temp_bg("C:/LETI/educatinal_practice/picture.jpg");
    QGraphicsPixmapItem *bg_item = scene->addPixmap(temp_bg);

    qreal scale_x = ui->plan_of_room->width() / qreal(temp_bg.width());
    qreal scale_y = ui->plan_of_room->height() / qreal(temp_bg.height());
    qreal scale_factor = qMin(scale_x, scale_y);
    bg_item->setScale(scale_factor);

    qreal center_x = (ui->plan_of_room->width() - temp_bg.width() * scale_factor) / 2;
    qreal center_y = (ui->plan_of_room->height() - temp_bg.height() * scale_factor) / 2;

    bg_item->setPos(center_x, center_y);
    ui->plan_of_room->setScene(scene);

    ui->plan_of_room->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->plan_of_room->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loading_xml(const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "File is not open!");
        return;
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        QMessageBox::critical(this, "Error", "File is not open!");
        file.close();
        return;
    }

    QDomElement room = doc.documentElement();
    QDomNodeList wall_list = room.elementsByTagName("wall");

    for (int i = 0; i < wall_list.size(); i++)
    {
        QDomNode temp = wall_list.at(i);
        if (temp.isElement())
        {
            QDomElement wall_item = temp.toElement();
            QDomNodeList point_list = wall_item.elementsByTagName("point");

            QPolygon polygon;
            for (int j = 0; j < point_list.size(); j++)
            {
                QDomElement point_item = point_list.at(j).toElement();
                int x = point_item.attribute("x").toInt();
                int y = point_item.attribute("y").toInt();
                polygon << QPoint(x, y);
            }

            scene->addPolygon(polygon, QPen(Qt::black, 3));
        }
    }
}


void MainWindow::on_load_pressed()
{
    scene->clear();
    ui->plan_of_room->centerOn(0,0);
    loading_xml("C:/LETI/educatinal_practice/Plan.xml");
}


void MainWindow::on_exit_pressed()
{
    exit(0);
}

