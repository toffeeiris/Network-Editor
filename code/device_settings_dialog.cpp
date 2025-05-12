#include "device_settings_dialog.h"
#include "ui_device_settings_dialog.h"

DeviceSettingsDialog::DeviceSettingsDialog(NetworkDevice *device, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeviceSettingsDialog)
    , device(device)
{
    ui->setupUi(this);
    setWindowTitle("Настройки сетевого устройства"); // Установка заголовка окна

    // Заполнение полей формы текущими значениями свойств устройства
    ui->producerNameEdit->setText(device->getProducerName());
    ui->deviceModelEdit->setText(device->getDeviceModel());
    ui->serialNumberEdit->setText(QString::number(device->getSerialNumber()));
    ui->serviceLifeSpin->setText(QString::number(device->getServiceLife()));
    ui->descriptionEdit->setText(device->getDescription());

    // Установка валидатора для ввода только целых чисел
    QIntValidator *intValidator = new QIntValidator(0, 999999, this);
    ui->serialNumberEdit->setValidator(intValidator);
    ui->serviceLifeSpin->setValidator(intValidator);

    // Подключение сигнала кнопки "Сохранить" к слоту onSaveClicked
    connect(ui->saveButton_2, &QPushButton::clicked, this, &DeviceSettingsDialog::onSaveClicked);

}

DeviceSettingsDialog::~DeviceSettingsDialog()
{
    delete ui;
}

void DeviceSettingsDialog::onSaveClicked()
{
    // Получаем данные из полей
    device->setProducerName(ui->producerNameEdit->text());
    device->setDeviceModel(ui->deviceModelEdit->text());
    device->setSerialNumber(ui->serialNumberEdit->text().toInt());
    device->setServiceLife(ui->serviceLifeSpin->text().toInt());
    device->setDescription(ui->descriptionEdit->text());

    // Сохраняем параметры устройства
    device->getFeatures();
    accept();  // Закрыть диалог
}
