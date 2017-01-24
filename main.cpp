#include <jack/transport.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
//#include <QProcess>
//#include <QAbstractItemModel>

//#include <ChannelStrip.h>
#include <ChannelStripModel.h>
#include <transport.h>
//#include <mixer.h>

jack_client_t* client;


int main(int argc, char *argv[]) {


    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlEngine      engine;

    if ((client = jack_client_open ("Mixr", JackNullOption, NULL)) == 0) {
        fprintf (stderr, "jack server not running?\n");
        return 1;
    }

    Mixr::ChannelStripModel m_Model(client);
    m_Model.setCallback(client);

    if (jack_activate (client)) {
        fprintf(stderr, "cannot activate client");
    }

    m_Model.add("Master", "nothing");
    for (int i = 11; i > 0; i--) {
        m_Model.add(QString().sprintf("Ch %03u", i), "");
    }


    engine.rootContext()->setContextProperty("MixrModel", &m_Model);


    Mixr::Transport m_transport(client);
    engine.rootContext()->setContextProperty("c_transport", &m_transport);


    QQmlComponent component(&engine, QUrl(QLatin1String("qrc:/main.qml")));
    QObject* object = component.create();


    return app.exec();
    delete object;
}


