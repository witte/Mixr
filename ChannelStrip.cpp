#include "ChannelStrip.h"

namespace Mixr {

ChannelStrip::ChannelStrip(jack_client_t* clientName, const QString& stripName, QObject* parentObj) : QObject(parentObj),
    client{clientName},
    name{stripName}
{
    input_port_1 = registerPort(name + "_in_1", JackPortIsInput);
    input_port_2 = registerPort(name + "_in_2", JackPortIsInput);
    output_port_1 = registerPort(name + "_out_1", JackPortIsOutput);
    output_port_2 = registerPort(name + "_out_2", JackPortIsOutput);
}

ChannelStrip::~ChannelStrip() { }

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

QString ChannelStrip::getName() const {
    return name;
}
void ChannelStrip::setName(const QString channelStripName) {
    name = channelStripName;
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

float ChannelStrip::getVolume()     const { return volume; }
float ChannelStrip::getVolumeL()    const { return volL;   }
float ChannelStrip::getVolumeR()    const { return volR;   }

bool ChannelStrip::getTempMute()    const { return tempMute; }
void ChannelStrip::setTempMute(const bool tempMuteValue) { tempMute = tempMuteValue; }


bool ChannelStrip::getMute() const { return mute; }
void ChannelStrip::setMute(const bool muteValue) { mute = muteValue; }

bool ChannelStrip::getSolo() const { return solo; }
void ChannelStrip::setSolo(const bool soloValue) { solo = soloValue; }

float ChannelStrip::getPan() const {
    return pan;
}

void ChannelStrip::setPan(const float panValue) {
    pan = panValue;
    setPortVolumes();
}

ChannelStrip* ChannelStrip::getParent() {
    return parentChannelStrip;
}

void ChannelStrip::setParent(ChannelStrip* newParentChannelStrip) {
    parentChannelStrip = newParentChannelStrip;
    level = parentChannelStrip->level+1;

    QString m_parentColors = "";
//    QVector<int> array(0);
//    array.append(color);

    m_parentColors += QString::number(color);

    ChannelStrip* m_parent = newParentChannelStrip;
    while (m_parent != nullptr) {

        if (m_parent->getLevel() > 0) {
            m_parentColors.append(",");
            m_parentColors.append( QString::number(m_parent->color) );

        }

        m_parent = m_parent->parentChannelStrip;
    }

    parentColors = m_parentColors;

    jack_connect ( client, jack_port_name(output_port_1), jack_port_name(parentChannelStrip->input_port_1) );
    jack_connect ( client, jack_port_name(output_port_2), jack_port_name(parentChannelStrip->input_port_2) );
}

uint ChannelStrip::getColor() const {
    return color;
}

void ChannelStrip::setColor(const uint colorValue) {
    color = colorValue;

    if (parentColors == "") parentColors = QString::number(color);;
}

uint ChannelStrip::getLevel() const {
    return level;
}

//QVector<int> ChannelStrip::getParentColors() const {
QString ChannelStrip::getParentColors() const {
    return parentColors;
}

jack_port_t* ChannelStrip::registerPort(const QString& portName, const JackPortFlags portFlags) const {
    return jack_port_register(client, portName.toLatin1().data(), JACK_DEFAULT_AUDIO_TYPE, portFlags, 0);
}

int ChannelStrip::connectFrom(const QString& portName, const int side) {
    const auto* m_portName = portName.toLatin1().data();

    if (side == 1)
        return jack_connect(client, m_portName, jack_port_name(input_port_1));

    else
        return jack_connect(client, m_portName, jack_port_name(input_port_2));

}

int ChannelStrip::disconnectFrom(const QString& portName, const int side) {
    const auto* m_portName = portName.toLatin1().data();

    if (side == 1)
        return jack_disconnect (client, m_portName, jack_port_name(input_port_1));

    else
        return jack_disconnect (client, m_portName, jack_port_name(input_port_2));

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

int ChannelStrip::connectTo(const QString& portName, const int side) {
    const auto* m_portName = portName.toLatin1().data();

    if (side == 1)
        return  jack_connect (client, jack_port_name(output_port_1), m_portName);

    else
        return  jack_connect (client, jack_port_name(output_port_2), m_portName);

}


int ChannelStrip::disconnectTo(const QString& portName, const int side) {
    const auto* m_portName = portName.toLatin1().data();

    if (side == 1)
        return jack_disconnect (client, jack_port_name(output_port_1), m_portName);

    else
        return jack_disconnect (client, jack_port_name(output_port_2), m_portName);

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

    QStringList a{};
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
