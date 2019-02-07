QT += quick serialport sql
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Add DEBUG for debug.cpp
CONFIG(release, debug|release):DEFINES += DEBUG=0 QT_NO_DEBUG_OUTPUT=1 QT_NO_WARNING_OUTPUT=1
else:CONFIG(debug, debug|release):DEFINES += DEBUG=1

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    src/main.cpp \
    src/debug.cpp \
    src/qml_port_list.cpp \
    src/qml_usb_sensor.cpp \
    src/qml_statistic.cpp \
    src/qml_config.cpp

HEADERS += \
    src/debug.h \
    src/qml_port_list.h \
    src/qml_usb_sensor.h \
    src/qml_statistic.h \
    src/qml_config.h

INCLUDEPATH += ..\SharedLibrary\release

win32: LIBS+= -L$$PWD/../SharedLibrary/release -lShared_mingw_32


# Application icon for win32
RC_ICONS = assets/app_icon.ico
