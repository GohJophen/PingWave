QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src\customPlot.cpp \
    src\main.cpp \
    src\mainWindow.cpp \
    src\scrollArea.cpp \
    src\widget.cpp

HEADERS += \
    include\customPlot.h \
    include\mainWindow.h \
    include\scrollArea.h \
    include\widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources\resources.qrc
