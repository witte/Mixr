#ifndef FADER_H
#define FADER_H

#include <QObject>
#include <jack/jack.h>


class Fader : public QObject
{
    Q_OBJECT

public:
    Fader(jack_client_t *j_client, QString name);

    jack_client_t *client;
    jack_port_t *input_port_1;
    jack_port_t *input_port_2;
    jack_port_t *output_port_1;
    jack_port_t *output_port_2;

    QString name;

    bool mute;
    float volume;
    float pan;
    float volL = 1;
    float volR = 1;

    float peakL = 0.0f;
    float peakR = 0.0f;

    ~Fader();

private:
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

#endif // FADER_H
