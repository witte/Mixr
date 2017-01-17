#include <QString>
#include <math.h>
#include "Fader.h"

namespace Mixr {

Fader::Fader(jack_client_t *j_client, QString name)
{
    this->client = j_client;
    this->volume = 1.0f;
    this->name = name;
    this->pan = 0.5f;

    QString s_input1 = name, s_input2 = name, s_output1 = name, s_output2 = name;
    s_input1.append("/in 1");
    s_input2.append("/in 2");
    s_output1.append("/out 1");
    s_output2.append("/out 2");


    input_port_1    = jack_port_register (client, s_input1.toLatin1().data(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    input_port_2    = jack_port_register (client, s_input2.toLatin1().data(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    output_port_1   = jack_port_register (client, s_output1.toLatin1().data(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    output_port_2   = jack_port_register (client, s_output2.toLatin1().data(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
}

Fader::~Fader()
{
    jack_deactivate(client);
    jack_port_unregister(client, input_port_1);
    jack_port_unregister(client, input_port_2);
    jack_port_unregister(client, output_port_1);
    jack_port_unregister(client, output_port_2);
    jack_client_close(client);
}

void Fader::connectFrom(QString jack_port) {

    jack_port_disconnect(client, input_port_1);
    jack_port_disconnect(client, input_port_2);

    const char *c_str2 = jack_port.toLatin1().data();
    jack_connect (client, c_str2, jack_port_name (input_port_1));

    QString suffix = jack_port.mid(jack_port.length()-2, 2);
    if (suffix == "_1") {
        jack_port.chop(2);
        jack_port.append("_2");

        c_str2 = jack_port.toLatin1().data();
        jack_connect (client, c_str2, jack_port_name (input_port_2));
    }
    else {
        jack_connect (client, jack_port.toLatin1().data(), jack_port_name (input_port_2));
    }
}

void Fader::connectTo(QString jack_port) {

    jack_port_disconnect(client, output_port_1);
    jack_port_disconnect(client, output_port_2);

    const char *c_str2 = jack_port.toLatin1().data();
    jack_connect (client, jack_port_name (output_port_1), c_str2);

    QString suffix = jack_port.mid(jack_port.length()-2, 2);
    if (suffix == "_1") {
        jack_port.chop(2);
        jack_port.append("_2");

        c_str2 = jack_port.toLatin1().data();
        jack_connect (client, jack_port_name (output_port_2), c_str2);
    }
}

void Fader::setVolume(float a)  { volume = a; setPortVolumes(); }
void Fader::setPan   (float a)  { pan    = a; setPortVolumes(); }
void Fader::setMute  (bool  a)  { mute   = a;  }
bool Fader::getMute  ()         { return mute; }
float Fader::getPan  ()         { return pan;  }

void Fader::setPortVolumes() {
    volL = volume * ((1 - pan) * M_PI_2); // equal power, -3db at center
    volR = volume * (pan * M_PI_2);
}


QStringList Fader::getJackPorts(JackPortFlags jackPortFlags) {

    QStringList a;
    const char **ports = jack_get_ports ( client, NULL, NULL, jackPortFlags );

    a.append( "none" );
    int i = 0;
    while (ports[i] != NULL) {
        bool add = true;

        foreach (QString s, port_names) {
            if (ports[i] == s) add = false;
        }

        if (add) a.append( ports[i] );
        i++;
    }
    jack_free (ports);

    return a;
}

QStringList Fader::getJackInputPorts()  { return getJackPorts(JackPortIsInput); }
QStringList Fader::getJackOutputPorts() { return getJackPorts(JackPortIsOutput); }

} // namespace Mixr
