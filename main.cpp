#include <jack/transport.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QProcess>

#include <ChannelStrip.h>
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

    Mixr::ChannelStrip m_ChannelStrip_01(client, "01");
    Mixr::ChannelStrip m_ChannelStrip_02(client, "02");
    Mixr::ChannelStrip m_ChannelStrip_03(client, "03");
    Mixr::ChannelStrip m_ChannelStrip_04(client, "04");

    Mixr::Mixer m_Mixer(client);
    m_Mixer.channelStrip_01 = &m_ChannelStrip_01;
    m_Mixer.channelStrip_02 = &m_ChannelStrip_02;
    m_Mixer.channelStrip_03 = &m_ChannelStrip_03;
    m_Mixer.channelStrip_04 = &m_ChannelStrip_04;

    engine.rootContext()->setContextProperty("c_ChannelStrip_01", m_Mixer.channelStrip_01);
    engine.rootContext()->setContextProperty("c_ChannelStrip_02", m_Mixer.channelStrip_02);
    engine.rootContext()->setContextProperty("c_ChannelStrip_03", m_Mixer.channelStrip_03);
    engine.rootContext()->setContextProperty("c_ChannelStrip_04", m_Mixer.channelStrip_04);

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


