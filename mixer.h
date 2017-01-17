#ifndef MIXER_H
#define MIXER_H

#include <jack/jack.h>
#include "fader.h"

namespace Mixr {

class Mixer
{
public:
    Mixer(jack_client_t *j_client);

    jack_client_t *client;

    Fader* fader_01;
    Fader* fader_02;
    Fader* fader_03;
    Fader* fader_04;

    void setCallback();

    static int process (jack_nframes_t nframes, void *arg);
};

} // namespace Mixr

#endif // MIXER_H
