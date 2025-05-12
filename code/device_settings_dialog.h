#ifndef DEVICE_SETTINGS_DIALOG_H
#define DEVICE_SETTINGS_DIALOG_H

#include <QDialog>
#include "network_device.h"

namespace Ui
{
class DeviceSettingsDialog;
}

class DeviceSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DeviceSettingsDialog(NetworkDevice *device, QWidget *parent = nullptr);
    ~DeviceSettingsDialog();

private slots:
    void onSaveClicked(); // Слот для обработки нажатия кнопки сохранения

private:
    Ui::DeviceSettingsDialog *ui;
    NetworkDevice *device;
};

#endif // DEVICE_SETTINGS_DIALOG_H
