#include <jack/transport.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>

#include "ChannelStripModel.h"
#include "transport.h"

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

    QQmlEngine::setObjectOwnership(&m_Model, QQmlEngine::CppOwnership);


    m_Model.setCallback(client);

    if (jack_activate (client)) {
        fprintf(stderr, "cannot activate client");
    }

    m_Model.add("Master", "-");

    m_Model.add("VOICE", "Master");

    m_Model.add("Dry", "VOICE");
    m_Model.add("FX", "VOICE");
    m_Model.add("Chorus", "VOICE");
    m_Model.add("__1", "Chorus");
    m_Model.add("__2", "__1");
    m_Model.add("__3", "__2");
    m_Model.add("__4", "__3");
    m_Model.add("__5", "__4");
    m_Model.add("__6", "__5");
    m_Model.add("__7", "__6");


    m_Model.add("DRUMS", "Master");

    m_Model.add("Drums", "DRUMS");
    m_Model.add("Room", "Drums");
    m_Model.add("OH", "Drums");
    m_Model.add("Floor Tom", "Drums");
    m_Model.add("Tom", "Drums");
    m_Model.add("Snare", "Drums");
    m_Model.add("Top", "Snare");
    m_Model.add("Bottom", "Snare");
    m_Model.add("Kick", "Drums");

    m_Model.add("Percussion", "DRUMS");
    m_Model.add("Cowbell", "Percussion");
    m_Model.add("Shaker", "Percussion");


//    for (int i = 300; i > 0; i--) {
//        m_Model.add(QString().sprintf("Ch %03u", i), "");
//    }



    engine.rootContext()->setContextProperty("MixrModel", &m_Model);
//    QVariant::fromValue(dataList)
//    engine.rootContext()->


    Mixr::Transport m_transport(client);
    engine.rootContext()->setContextProperty("c_transport", &m_transport);


    QQmlComponent component(&engine, QUrl(QLatin1String("qrc:/main.qml")));
    QObject* object = component.create();

    QQmlEngine::setObjectOwnership(&m_Model, QQmlEngine::CppOwnership);

    return app.exec();
    delete object;
}


