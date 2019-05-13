#include <QApplication>
#include <QQmlApplicationEngine>
#include <QIcon>

#include "qml_port_list.h"
#include "qml_config.h"
#include "qml_usb_sensor.h"
#include "qml_statistic.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterType <CPortList>("com.dogratian.qml.PortList", 1, 0, "PortList");
    qmlRegisterType <CConfig>("com.dogratian.qml.Config", 1, 0, "Config");
    qmlRegisterType <CUsbSensor>("com.dogratian.qml.UsbSensor", 1, 0, "UsbSensor");
    qmlRegisterType <CStatistic>("com.dogratian.qml.Statistic", 1, 0, "Statistic");

    QApplication::setOrganizationName("DogRatIan");
    app.setWindowIcon (QIcon("qrc:/assets/app_icon.png"));


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
