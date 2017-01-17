#ifndef FADER_H
#define FADER_H

#include <QObject>
#include <jack/jack.h>

namespace Mixr {

class Fader : public QObject
{
    Q_OBJECT

public:
    Fader(jack_client_t *j_client, const QString& name);
    ~Fader();

    jack_client_t *client{nullptr};
    jack_port_t *input_port_1{nullptr};
    jack_port_t *input_port_2{nullptr};
    jack_port_t *output_port_1{nullptr};
    jack_port_t *output_port_2{nullptr};

    QString name;

    bool mute{false};
    float volume{1.0f};
    float pan{0.5f};
    float volL{1.0f};
    float volR{1.0f};
    float peakL{0.0f};
    float peakR{0.0f};

private:
    jack_port_t* registerPort(const QString& name, const JackPortFlags portFlags) const;

    QStringList port_names;

public slots:
    void connectFrom(QString jack_port);
    void connectTo(QString jack_port);
    void setVolume(float a);
    void setMute(bool a);
    void setPan(float a);

    bool getMute();
    QStringList getJackInputPorts();
    QStringList getJackOutputPorts();

protected:
    float getPan();

private slots:
    QStringList getJackPorts(JackPortFlags jackPortFlags);
    void setPortVolumes();

};

} // namespace Mixr

#endif // FADER_H
