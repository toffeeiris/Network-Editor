QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    armchair.cpp \
    cabinet.cpp \
    chair.cpp \
    computer.cpp \
    device_settings_dialog.cpp \
    door.cpp \
    furniture.cpp \
    ip_camera.cpp \
    ip_phone.cpp \
    main.cpp \
    mainwindow.cpp \
    network_device.cpp \
    parser.cpp \
    printer.cpp \
    room_layout.cpp \
    room_object.cpp \
    router.cpp \
    server.cpp \
    table.cpp \
    wall.cpp \
    window.cpp

HEADERS += \
    armchair.h \
    cabinet.h \
    chair.h \
    computer.h \
    device_settings_dialog.h \
    door.h \
    furniture.h \
    ip_camera.h \
    ip_phone.h \
    mainwindow.h \
    network_device.h \
    parser.h \
    printer.h \
    room_layout.h \
    room_object.h \
    router.h \
    server.h \
    table.h \
    wall.h \
    window.h

FORMS += \
    device_settings_dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES +=
