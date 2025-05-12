#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "orbit_calculation.h"
#include "sql_database.h"
#include "net_bases.h"
#include "net_new_elements.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<orbit_calculation>("calc.orbit_calculation", 1, 0, "OrbitCalculator");
    qmlRegisterType<sql_database>("sql.interplanetary_net", 1, 0, "Interplanetary_net");
    qmlRegisterType<net_bases>("net.net_bases",1,0,"Net_Base");
    qmlRegisterType<net_new_elements>("net.net_new_elements",1,0,"Net_new_elems");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/planet_init_project/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
