#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDomDocument>
#include <QFile>
#include <QGraphicsPolygonItem>
#include <QMessageBox>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_load_pressed();
    void on_exit_pressed();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    void loading_xml(const QString &filepath);
};

#endif // MAINWINDOW_H
