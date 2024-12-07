QT       += core gui printsupport opengl concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/mainwindow.h

FORMS += \
    src/mainwindow.ui

INCLUDEPATH += \
    src

RESOURCES += \
    src/style/style.qss \
    src/style/style.sass \
    src/images/

DEFINES += QCUSTOMPLOT_USE_OPENGL

include(src/components/components.pri)
include(src/pages/pages.pri)
include(src/wizardPages/wizardPages.pri)
include(src/plotComponents/plotComponents.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
