#include "ChannelStrip.h"

#include <qdebug.h>

namespace Mixr {

ChannelStrip::ChannelStrip(jack_client_t *j_client, const QString& name)
    : client{j_client},
      name{name}
{
    input_port_1 = registerPort(name + "/in 1", JackPortIsInput);
    input_port_2 = registerPort(name + "/in 2", JackPortIsInput);
    output_port_1 = registerPort(name + "/out 1", JackPortIsOutput);
    output_port_2 = registerPort(name + "/out 2", JackPortIsOutput);
}

ChannelStrip::~ChannelStrip()
{

}

jack_port_t* ChannelStrip::getInputPort1() {
    return input_port_1;
}

jack_port_t* ChannelStrip::getInputPort2() {
    return input_port_2;
}

jack_port_t* ChannelStrip::getOutputPort1() {
    return output_port_1;
}

jack_port_t* ChannelStrip::getOutputPort2() {
    return output_port_2;
}

float ChannelStrip::getPeakL() const {
    return peakL;
}

void ChannelStrip::setPeakL(const float peak) {
    peakL = peak;
}

float ChannelStrip::getPeakR() const {
    return peakR;
}

void ChannelStrip::setPeakR(const float peak) {
    peakR = peak;
}

float ChannelStrip::getVolumeL() const {
    return volL;
}

float ChannelStrip::getVolumeR() const {
    return volR;
}

bool ChannelStrip::isMuted() const {
    return mute;
}

void ChannelStrip::isMuted(const bool isMute) {
    mute = isMute;
}

float ChannelStrip::getPan() const {
    return pan;
}

void ChannelStrip::setPan(const float panValue) {
    pan = panValue;
    setPortVolumes();
}

jack_port_t* ChannelStrip::registerPort(const QString& name, const JackPortFlags portFlags) const {
    return jack_port_register(client, name.toLatin1().data(), JACK_DEFAULT_AUDIO_TYPE, portFlags, 0);
}

void ChannelStrip::connectFrom(const QString& portName) {
    jack_port_disconnect(client, input_port_1);
    jack_port_disconnect(client, input_port_2);

    // connect input port 1
    const auto* portName1 = portName.toLatin1().data();
    jack_connect(client, portName1, jack_port_name(input_port_1));

    // connect input port 2
    const auto* portName2 = portName1;
    if (hasSuffixOne(portName)) {
        const auto portNameSuffix2 = ChangeSuffixToTwo(portName);
        portName2 = portNameSuffix2.toLatin1().data();
    }

    jack_connect(client, portName2, jack_port_name(input_port_2));
}

void ChannelStrip::connectTo(const QString& portName) {
    jack_port_disconnect(client, output_port_1);
    jack_port_disconnect(client, output_port_2);

    // connect output port 1
    const auto* portName1 = portName.toLatin1().data();
    jack_connect(client, jack_port_name(output_port_1), portName1);

    // connect output port 2
    if (hasSuffixOne(portName)) {
        const auto portNameSuffix2 = ChangeSuffixToTwo(portName);
        const auto* portName2 = portNameSuffix2.toLatin1().data();
        jack_connect(client, jack_port_name(output_port_2), portName2);
    }
}

bool ChannelStrip::hasSuffixOne(const QString& portName) const {
    return portName.mid(portName.length()-2, 2) == "_1";
}

QString ChannelStrip::ChangeSuffixToTwo(const QString& portName) const {
    QString str{portName};
    str.chop(2);
    str.append("_2");

    return str;
}

void ChannelStrip::setVolume(const float volumeValue)  {
    volume = volumeValue;
    setPortVolumes();
}

void ChannelStrip::setPortVolumes() {
    volL = volume * ((1 - pan) * M_PI_2); // equal power, -3db at center
    volR = volume * (pan * M_PI_2);
}

QStringList ChannelStrip::getJackPorts(JackPortFlags jackPortFlags) {
    const auto** ports = jack_get_ports(client, NULL, NULL, jackPortFlags);

    QStringList a{QString{"none"}};
    int i{0};
    while (ports[i] != NULL) {
        bool add = true;
        foreach (QString s, port_names) {
            if (ports[i] == s) {
                add = false;
            }
        }

        if (add) {
            a.append(ports[i]);
        }
        i++;
    }

    jack_free(ports);

    return a;
}

QStringList ChannelStrip::getJackInputPorts()  {
    return getJackPorts(JackPortIsInput);
}

QStringList ChannelStrip::getJackOutputPorts() {
    return getJackPorts(JackPortIsOutput);
}

} // namespace Mixr
