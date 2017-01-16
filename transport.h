#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>
#include <jack/jack.h>
#include <jack/transport.h>

class Transport : public QObject
{
    Q_OBJECT

public:
    Transport(jack_client_t *client);

    jack_client_t *client;
    float bpm;


public slots:
    void start();
    void stop();
    void rewind();

    QString getTime();
    QString getBBT();

};

#endif // TRANSPORT_H
