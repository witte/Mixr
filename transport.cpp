#include "transport.h"
#include <QTime>
#include <QString>
#include <jack/jack.h>
#include <jack/transport.h>

Transport::Transport(jack_client_t *client) { this->client = client; }

void Transport::rewind() { jack_transport_stop (client); jack_transport_locate(client, 0); }
void Transport::start()  { jack_transport_start(client); }
void Transport::stop()   { jack_transport_stop (client); }

QString Transport::getBBT() {
    jack_position_t pos;
    jack_transport_query(client, &pos);

    if (pos.valid)
        return QString().sprintf("%u.%u.%03u", pos.bar, pos.beat, pos.tick);
    else
        return "--.--.---";
}

QString Transport::getTime() {
    jack_position_t pos;
    jack_transport_query(client, &pos);

    float a = float(pos.frame) / float(pos.frame_rate) * 1000;
    QTime qTime(0, 0, 0, 0);
    qTime = qTime.addMSecs(a);

    return qTime.toString("HH:mm:ss.zzz");
}
