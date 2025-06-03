QT       += core gui printsupport opengl concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/app/main.cpp \
    src/app/mainwindow.cpp

HEADERS += \
    src/app/mainwindow.h

FORMS += \
    src/app/mainwindow.ui

INCLUDEPATH += \
    src

RESOURCES += \
    src/resources/style/style.qss \
    src/resources/style/style.sass \
    src/resources/images/

DEFINES += QCUSTOMPLOT_USE_OPENGL

include(src/ui/components/components.pri)
include(src/ui/dialogs/dialogs.pri)
include(src/ui/pages/pages.pri)
include(src/ui/wizardPages/wizardPages.pri)
include(src/core/core.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
