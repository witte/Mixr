#ifndef FADER_H
#define FADER_H

#include <QObject>
#include <QString>

#include <jack/jack.h>

class QStringList;

namespace Mixr {

class ChannelStrip : public QObject
{
    Q_OBJECT

public:
    ChannelStrip(jack_client_t *j_client, const QString& name);
    ~ChannelStrip();


public slots:
    jack_port_t* getInputPort1();
    jack_port_t* getInputPort2();
    jack_port_t* getOutputPort1();
    jack_port_t* getOutputPort2();

    QString getName() const;
    void setName(const QString channelStripName);

    float getPeakL() const;
    void setPeakL(const float peak);

    float getPeakR() const;
    void setPeakR(const float peak);

    float getVolume() const;
    float getVolumeL() const;
    float getVolumeR() const;

    bool getTempMute() const;
    void setTempMute(const bool tempMuteValue);

    bool getMute() const;
    void setMute(const bool muteValue);

    bool getSolo() const;
    void setSolo(const bool soloValue);

    float getPan() const;
    void setPan(const float panValue);

    ChannelStrip* getParent();
    void setParent(ChannelStrip* parentChannelStrip);

    uint getColor() const;
    void setColor(const uint colorValue);

    uint getLevel() const;

    void connectFrom(const QString& portName);
    int connectFrom(const QString& portName, const int side);
    int disconnectFrom(const QString& portName, const int side);
    void connectTo(const QString& portName);
    int connectTo(const QString& portName, const int side);
    int disconnectTo(const QString& portName, const int side);
    void setVolume(const float volumeValue);
    QStringList getJackInputPorts();
    QStringList getJackOutputPorts();

private:
    jack_port_t* registerPort(const QString& name, const JackPortFlags portFlags) const;
    QStringList getJackPorts(JackPortFlags jackPortFlags);
    void setPortVolumes();

    bool hasSuffixOne(const QString& portName) const;
    QString ChangeSuffixToTwo(const QString& portName) const;

    jack_client_t *client{nullptr};
    jack_port_t *input_port_1{nullptr};
    jack_port_t *input_port_2{nullptr};
    jack_port_t *output_port_1{nullptr};
    jack_port_t *output_port_2{nullptr};

    ChannelStrip *parent{nullptr};

    QString name;
    QStringList port_names{};

    bool tempMute{false};
    bool mute{false};
    bool solo{false};
    float volume{1.0f};
    float pan{0.5f};
    float volL{1.0f};
    float volR{1.0f};
    float peakL{0.0f};
    float peakR{0.0f};

    uint color{0};
    uint level{0};
};

} // namespace Mixr

#endif // FADER_H
