#include "mixer.h"

#include <jack/jack.h>
#include <jack/transport.h>

namespace Mixr {

static int processChannelStrip (jack_nframes_t nframes, void *arg)
{
    ChannelStrip* m_ChannelStrip = (ChannelStrip*)arg;

    jack_default_audio_sample_t *in_1  = (jack_default_audio_sample_t*)jack_port_get_buffer (m_ChannelStrip->getInputPort1(),  nframes);
    jack_default_audio_sample_t *in_2  = (jack_default_audio_sample_t*)jack_port_get_buffer (m_ChannelStrip->getInputPort2(),  nframes);
    jack_default_audio_sample_t *out_1 = (jack_default_audio_sample_t*)jack_port_get_buffer (m_ChannelStrip->getOutputPort1(), nframes);
    jack_default_audio_sample_t *out_2 = (jack_default_audio_sample_t*)jack_port_get_buffer (m_ChannelStrip->getOutputPort2(), nframes);

    m_ChannelStrip->setPeakL(0.0f);
    m_ChannelStrip->setPeakR(0.0f);

    if (!m_ChannelStrip->isMuted()) {
        float volL = m_ChannelStrip->getVolumeL();
        float volR = m_ChannelStrip->getVolumeR();

        for (unsigned int i = 0; i < nframes; i++) {
            out_1[i] = in_1[i] * volL;
            out_2[i] = in_2[i] * volR;

            if (out_1[i] > m_ChannelStrip->getPeakL()) m_ChannelStrip->setPeakL(out_1[i]);
            if (out_2[i] > m_ChannelStrip->getPeakR()) m_ChannelStrip->setPeakR(out_2[i]);
        }

    } else {
        memset (out_1, 0, sizeof (jack_default_audio_sample_t) * nframes);
        memset (out_2, 0, sizeof (jack_default_audio_sample_t) * nframes);
    }

    return 0;
}

static int processMixer (jack_nframes_t nframes, void *arg) {
    Mixer* m_Mixer = (Mixer*)arg;

    processChannelStrip (nframes, m_Mixer->channelStrip_01);
    processChannelStrip (nframes, m_Mixer->channelStrip_02);
    processChannelStrip (nframes, m_Mixer->channelStrip_03);
    processChannelStrip (nframes, m_Mixer->channelStrip_04);

    return 0;
}




void Mixer::setCallback() {
    jack_set_process_callback (client, processMixer, this);
}




Mixer::Mixer(jack_client_t *clientName) :
    client {clientName}
{

}

Mixer::~Mixer()
{
    jack_deactivate(client);

    unregisterChannelStripPorts(channelStrip_01);
    unregisterChannelStripPorts(channelStrip_02);
    unregisterChannelStripPorts(channelStrip_03);
    unregisterChannelStripPorts(channelStrip_04);

    jack_client_close(client);
}

void Mixer::unregisterChannelStripPorts(ChannelStrip *channelStrip)
{
    jack_port_unregister(client, channelStrip->getInputPort1());
    jack_port_unregister(client, channelStrip->getInputPort2());
    jack_port_unregister(client, channelStrip->getOutputPort1());
    jack_port_unregister(client, channelStrip->getOutputPort2());
}

} // namespace Mixr
