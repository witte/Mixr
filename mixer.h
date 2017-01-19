#ifndef MIXER_H
#define MIXER_H

#include <jack/jack.h>
#include "ChannelStrip.h"

namespace Mixr {

class Mixer
{
public:
    Mixer(jack_client_t *j_client);
    ~Mixer();

    jack_client_t *client;

    ChannelStrip* channelStrip_01;
    ChannelStrip* channelStrip_02;
    ChannelStrip* channelStrip_03;
    ChannelStrip* channelStrip_04;

    void setCallback();

private:
    void unregisterChannelStripPorts(ChannelStrip* channelStrip);

    static int process (jack_nframes_t nframes, void *arg);
};

} // namespace Mixr

#endif // MIXER_H
