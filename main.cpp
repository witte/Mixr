#include <jack/transport.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QProcess>

#include <Fader.h>
#include <transport.h>
#include <mixer.h>

jack_client_t *client;


int main(int argc, char *argv[]) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlEngine      engine;

    if ((client = jack_client_open ("QMixer", JackNullOption, NULL)) == 0) {
        fprintf (stderr, "jack server not running?\n");
        return 1;
    }

    Mixr::Fader m_Fader_01(client, "01");
    Mixr::Fader m_Fader_02(client, "02");
    Mixr::Fader m_Fader_03(client, "03");
    Mixr::Fader m_Fader_04(client, "04");

    Mixr::Mixer m_Mixer(client);
    m_Mixer.fader_01 = &m_Fader_01;
    m_Mixer.fader_02 = &m_Fader_02;
    m_Mixer.fader_03 = &m_Fader_03;
    m_Mixer.fader_04 = &m_Fader_04;

    engine.rootContext()->setContextProperty("c_fader_01", m_Mixer.fader_01);
    engine.rootContext()->setContextProperty("c_fader_02", m_Mixer.fader_02);
    engine.rootContext()->setContextProperty("c_fader_03", m_Mixer.fader_03);
    engine.rootContext()->setContextProperty("c_fader_04", m_Mixer.fader_04);

    Mixr::Transport m_transport(client);
    engine.rootContext()->setContextProperty("c_transport", &m_transport);


    m_Mixer.setCallback();


    if (jack_activate (client)) {
        fprintf(stderr, "cannot activate client");
    }


    QQmlComponent   component(&engine, QUrl(QLatin1String("qrc:/main.qml")));
    QObject *       object = component.create();


    return app.exec();
    delete object;
}


