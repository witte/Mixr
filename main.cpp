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

    if ((client = jack_client_open ("Mixr", JackNullOption, NULL)) == 0) {
        fprintf (stderr, "jack server not running?\n");
        return 1;
    }

    Mixr::ChannelStrip m_ChannelStrip_Kick  (client, "Kick");
    Mixr::ChannelStrip m_ChannelStrip_Snare (client, "Snare");
    Mixr::ChannelStrip m_ChannelStrip_Hat   (client, "Hat");
    Mixr::ChannelStrip m_ChannelStrip_OH    (client, "OH");

    Mixr::ChannelStrip m_ChannelStrip_Drums (client, "Drums");

    Mixr::Mixer m_Mixer(client);
    m_Mixer.channelStrip_01 = &m_ChannelStrip_Kick;
    m_Mixer.channelStrip_02 = &m_ChannelStrip_Snare;
    m_Mixer.channelStrip_03 = &m_ChannelStrip_Hat;
    m_Mixer.channelStrip_04 = &m_ChannelStrip_OH;
    m_Mixer.channelStrip_05 = &m_ChannelStrip_Drums;


    engine.rootContext()->setContextProperty("c_ChannelStrip_01", m_Mixer.channelStrip_01);
    engine.rootContext()->setContextProperty("c_ChannelStrip_02", m_Mixer.channelStrip_02);
    engine.rootContext()->setContextProperty("c_ChannelStrip_03", m_Mixer.channelStrip_03);
    engine.rootContext()->setContextProperty("c_ChannelStrip_04", m_Mixer.channelStrip_04);
    engine.rootContext()->setContextProperty("c_ChannelStrip_05", m_Mixer.channelStrip_05);


    Mixr::Transport m_transport(client);
    engine.rootContext()->setContextProperty("c_transport", &m_transport);


    m_Mixer.setCallback();

    if (jack_activate (client)) {
        fprintf(stderr, "cannot activate client");
    }

    m_ChannelStrip_Kick.setParent(&m_ChannelStrip_Drums);
    m_ChannelStrip_Snare.setParent(&m_ChannelStrip_Drums);
    m_ChannelStrip_Hat.setParent(&m_ChannelStrip_Drums);
    m_ChannelStrip_OH.setParent(&m_ChannelStrip_Drums);

    jack_connect ( client, jack_port_name(m_ChannelStrip_Drums.getOutputPort1()), "system:playback_1" );
    jack_connect ( client, jack_port_name(m_ChannelStrip_Drums.getOutputPort2()), "system:playback_2" );


    QQmlComponent   component(&engine, QUrl(QLatin1String("qrc:/main.qml")));
    QObject *       object = component.create();


    return app.exec();
    delete object;
}


