#ifndef FADER_H
#define FADER_H

#include <QObject>
#include <QString>

#include <jack/jack.h>

class QStringList;

namespace Mixr {

class Fader : public QObject
{
    Q_OBJECT

public:
    Fader(jack_client_t *j_client, const QString& name);
    ~Fader();

public slots:
    jack_port_t* getInputPort1();
    jack_port_t* getInputPort2();
    jack_port_t* getOutputPort1();
    jack_port_t* getOutputPort2();

    float getPeakL() const;
    void setPeakL(const float peak);

    float getPeakR() const;
    void setPeakR(const float peak);

    float getVolumeL() const;
    float getVolumeR() const;

    bool isMuted() const;
    void isMuted(const bool isMute);

    float getPan() const;
    void setPan(const float panValue);

    void connectFrom(const QString& portName);
    void connectTo(const QString& portName);
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

    QString name;
    QStringList port_names{};

    bool mute{false};
    float volume{1.0f};
    float pan{0.5f};
    float volL{1.0f};
    float volR{1.0f};
    float peakL{0.0f};
    float peakR{0.0f};
};

} // namespace Mixr

#endif // FADER_H
