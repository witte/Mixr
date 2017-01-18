#include "mixer.h"

#include <jack/jack.h>
#include <jack/transport.h>

namespace Mixr {

//static void processs (jack_transport_state_t state, jack_nframes_t nframes, jack_position_t *pos, int, void *arg) {}
static int processFader (jack_nframes_t nframes, void *arg)
{
    Fader* m_fader = (Fader*)arg;

    jack_default_audio_sample_t *in_1  = (jack_default_audio_sample_t*)jack_port_get_buffer (m_fader->getInputPort1(),  nframes);
    jack_default_audio_sample_t *in_2  = (jack_default_audio_sample_t*)jack_port_get_buffer (m_fader->getInputPort2(),  nframes);
    jack_default_audio_sample_t *out_1 = (jack_default_audio_sample_t*)jack_port_get_buffer (m_fader->getOutputPort1(), nframes);
    jack_default_audio_sample_t *out_2 = (jack_default_audio_sample_t*)jack_port_get_buffer (m_fader->getOutputPort2(), nframes);

    m_fader->setPeakL(0.0f);
    m_fader->setPeakR(0.0f);

    if (!m_fader->isMuted()) {
        float volL = m_fader->getVolumeL();
        float volR = m_fader->getVolumeR();

        for (unsigned int i = 0; i < nframes; i++) {
            out_1[i] = in_1[i] * volL;
            out_2[i] = in_2[i] * volR;

            if (out_1[i] > m_fader->getPeakL()) m_fader->setPeakL(out_1[i]);
            if (out_2[i] > m_fader->getPeakR()) m_fader->setPeakR(out_2[i]);
        }

    } else {
        memset (out_1, 0, sizeof (jack_default_audio_sample_t) * nframes);
        memset (out_2, 0, sizeof (jack_default_audio_sample_t) * nframes);
    }

    return 0;
}

static int processMixer (jack_nframes_t nframes, void *arg) {
    Mixer* m_mixer = (Mixer*)arg;

    processFader (nframes, m_mixer->fader_01);
    processFader (nframes, m_mixer->fader_02);
    processFader (nframes, m_mixer->fader_03);
    processFader (nframes, m_mixer->fader_04);

    return 0;
}




void Mixer::setCallback() {
    jack_set_process_callback (client, processMixer, this);
}




Mixer::Mixer(jack_client_t *j_client)
{
    this->client = j_client;

}

} // namespace Mixr
