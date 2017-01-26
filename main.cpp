#include <jack/transport.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QThread>

#include "ChannelStripModel.h"
#include "ChannelStrip.h"
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

    Mixr::ChannelStripModel* m_Model = new Mixr::ChannelStripModel(client, &app);

    QQmlEngine::setObjectOwnership(m_Model, QQmlEngine::CppOwnership);


    m_Model->setCallback(client);

    if (jack_activate (client)) {
        fprintf(stderr, "cannot activate client");
    }

    m_Model->add("Master", "-", &app);

    m_Model->add("VOICE", "Master", &app);
        m_Model->add("Dry", "VOICE", &app);
        m_Model->add("FX", "VOICE", &app);

    m_Model->add("DRUMS", "Master", &app);
        m_Model->add("Drums", "DRUMS", &app);
            m_Model->add("Room", "Drums", &app);
            m_Model->add("OH", "Drums", &app);
            m_Model->add("Floor Tom", "Drums", &app);
            m_Model->add("Tom", "Drums", &app);
            m_Model->add("Snare", "Drums", &app);
            m_Model->add("Top", "Snare", &app);
            m_Model->add("Bottom", "Snare", &app);
            m_Model->add("Kick", "Drums", &app);

        m_Model->add("Percussion", "DRUMS", &app);
            m_Model->add("Cowbell", "Percussion", &app);
            m_Model->add("Shaker", "Percussion", &app);

    // For testing purposes, make sure that the jack server is configured to allow that many ports!
    // (each channelstrip creates 4 ports)
    for (int i = 150; i > 0; i--) {
        m_Model->add(QString().sprintf("Ch %03u", i), "", &app);
    }


    engine.rootContext()->setContextProperty("MixrModel", m_Model);


    Mixr::Transport m_transport(client);
    engine.rootContext()->setContextProperty("c_transport", &m_transport);


    QQmlComponent component(&engine, QUrl(QLatin1String("qrc:/main.qml")));
    QObject* object = component.create();

    QQmlEngine::setObjectOwnership(m_Model, QQmlEngine::CppOwnership);

    return app.exec();
    delete object;
}


