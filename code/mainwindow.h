#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parser.h"

#include <QApplication>
#include <QMainWindow>
#include <QFileDialog>
#include <QScreen>

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
    // Слоты для кнопок интерфейса
    void on_load_pressed();    // Загрузка плана
    void on_exit_pressed();    // Выход из приложения
    void on_save_pressed();    // Сохранение плана
    void on_inf_pressed();     // Показ информации об управлении

    // Слоты для работы с объектами
    void handleDragObject(RoomObject* source, const QPointF& scenePos);    // Обработка перетаскивания
    void handleDroppedObject(RoomObject* source, const QPointF& scenePos); // Обработка отпускания
    void handleDeleteObject(RoomObject* source);   // Удаление объекта


private:
    Ui::MainWindow *ui;

    // Состояние приложения
    bool isPlanLoaded = false;  // Загружен ли план
    RoomObject* currDrag = nullptr;  // Текущий перетаскиваемый объект
    QGraphicsPixmapItem* currPreview = nullptr;  // Предпросмотр объекта

    Parser parser;  // Парсер XML
    QGraphicsScene *scene;  // Графическая сцена

    // Вспомогательный метод для работы с иконоками кнопок
    void updateIcon(QPushButton *button, const QString& path, const QString& tip);

};

#endif // MAINWINDOW_H
