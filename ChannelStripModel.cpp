#include "ChannelStripModel.h"
#include "ChannelStrip.h"

#include <QQmlEngine>

#include "mixer.h"

namespace Mixr {


ChannelStripModel::ChannelStripModel(jack_client_t* jackClientName, QObject* parent) : QAbstractListModel(parent)
{
    jackClient = jackClientName;
}

ChannelStripModel::~ChannelStripModel() {
    jack_deactivate(jackClient);

    for (int i = 0; i < m_ChannelStrips.length(); i++) {

        jack_port_unregister(jackClient, m_ChannelStrips[i]->getInputPort1());
        jack_port_unregister(jackClient, m_ChannelStrips[i]->getInputPort2());
        jack_port_unregister(jackClient, m_ChannelStrips[i]->getOutputPort1());
        jack_port_unregister(jackClient, m_ChannelStrips[i]->getOutputPort2());

    }

    jack_client_close(jackClient);
}

QHash<int,QByteArray> ChannelStripModel::roleNames() const // Q_DECL_OVERRIDE
{
     QHash<int, QByteArray> result;
     result.insert(NameRole,      QByteArrayLiteral("csName"));
     result.insert(PanRole,          QByteArrayLiteral("csPan"));
     result.insert(VolumeRole,       QByteArrayLiteral("csVolume"));
     result.insert(TempMuteRole,     QByteArrayLiteral("csTempMute"));
     result.insert(MuteRole,         QByteArrayLiteral("csMute"));
     result.insert(SoloRole,         QByteArrayLiteral("csSolo"));
     result.insert(ParentRole,       QByteArrayLiteral("csParent"));
     result.insert(LevelRole,        QByteArrayLiteral("csLevel"));
     result.insert(ColorRole,        QByteArrayLiteral("csColor"));
     result.insert(ParentColorsRole, QByteArrayLiteral("csParentColors"));
     result.insert(VisibleRole,      QByteArrayLiteral("csVisible"));
     result.insert(Widgets,          QByteArrayLiteral("csWidgets"));
     result.insert(ChildrenRole,     QByteArrayLiteral("csChildren"));
     return result;
}

QVariant ChannelStripModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const //Q_DECL_OVERRIDE
{
    if (!index.isValid())
        return QVariant();

    ChannelStrip* item = m_ChannelStrips[index.row()];

    switch (role) {
        case NameRole:          return QVariant::fromValue(item->getName()); break;
        case PanRole:           return QVariant::fromValue(item->getPan()); break;
        case VolumeRole:        return QVariant::fromValue(item->getVolume()); break;
        case TempMuteRole:      return QVariant::fromValue(item->getTempMute()); break;
        case MuteRole:          return QVariant::fromValue(item->getMute()); break;
        case SoloRole:          return QVariant::fromValue(item->getSolo()); break;
        case LevelRole:         return QVariant::fromValue(item->getLevel()); break;
        case ColorRole:         return QVariant::fromValue(item->getColor()); break;
        case ParentColorsRole:  return QVariant::fromValue(item->getParentColors()); break;
        case VisibleRole:
        case Widgets:
        case ChildrenRole:
        default:
            break;
    }
    return QVariant();
}

bool ChannelStripModel::setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) //Q_DECL_OVERRID
{
    if ( (index.row() >= 0) && (index.row() < m_ChannelStrips.length()) ) {

        ChannelStrip* item = m_ChannelStrips[index.row()];

        QString sRole = "";

        switch (role) {
            case NameRole:      item->setName(value.toString()); sRole = "name";  break;
            case PanRole:       item->setPan(value.toFloat()); sRole = "pan"; break;
            case VolumeRole:    item->setVolume(value.toFloat()); sRole = "volume";  break;
            case TempMuteRole:  item->setTempMute(value.toBool()); sRole = "tempMute"; break;
            case MuteRole:      item->setMute(value.toBool());  sRole = "mute"; break;
            case SoloRole:      item->setSolo(value.toBool());  sRole = "solo"; break;
            case ColorRole:     item->setColor(value.toUInt()); break;
            case ParentRole:
            case LevelRole:
            case VisibleRole:
            case Widgets:
            case ChildrenRole:
            default:
                break;
        }

        emit dataChanged(index, index);
        return true;
    }


    return false;

}

bool ChannelStripModel::setMute  (int row, const bool  &value) { return setData(createIndex(row, 0), value, MuteRole); }
bool ChannelStripModel::setPan   (int row, const float &value) { return setData(createIndex(row, 0), value, PanRole); }
bool ChannelStripModel::setVolume(int row, const float &value) {
    if ( (row >= 0) && (row < m_ChannelStrips.length()) ) {

        setData(createIndex(row, 0), value, VolumeRole);
        return true;

    }
    else return false;
}

bool ChannelStripModel::setColor (int row, const char &value) { return setData(createIndex(row, 0), value, ColorRole); }

int ChannelStripModel::rowCount() const {
    return m_ChannelStrips.length();
}

//void ChannelStripModel::add(ChannelStrip* channelStrip, ChannelStrip* channelStripParent) {

//    if (channelStripParent != nullptr)
//        channelStrip->setParent(channelStripParent);

////    m_ChannelStrips.insert(0, channelStrip);
//}

void ChannelStripModel::add(const QString channelStripName, const QString channelStripParentName) {

    m_ChannelStrips.append(new ChannelStrip(jackClient, channelStripName));


    Mixr::ChannelStrip* m_Channel = m_ChannelStrips.last();
    QQmlEngine::setObjectOwnership(m_ChannelStrips.last(), QQmlEngine::CppOwnership);


    m_Channel->setColor( (rand() % 8) );


    if (channelStripParentName == "") {
        // We connect it to the Master CS if no parent is given
        m_Channel->setParent( m_ChannelStrips.first() );
    }
    else {
        Mixr::ChannelStrip* m_ChannelParent = getChannelStripByName(channelStripParentName);

        if (m_ChannelParent != nullptr)
            m_Channel->setParent(m_ChannelParent);

    }

    if (channelStripName == "Master") {
        m_Channel->connectTo("system:playback_1", 1);
        m_Channel->connectTo("system:playback_2", 2);
    }


}

QStringList ChannelStripModel::getJackOutputPorts() {
    const auto** ports = jack_get_ports(jackClient, NULL, NULL, JackPortIsOutput);

    QStringList a{};
    int i{0};
    while (ports[i] != NULL) {
        bool add = true;
//        foreach (QString s, port_names) {
//            if (ports[i] == s) {
//                add = false;
//            }
//        }

        if (add) {
            a.append(ports[i]);
        }
        i++;
    }

    jack_free(ports);

    return a;
}

int ChannelStripModel::connectFrom(int row, const QString& portName, const int side) {
    ChannelStrip* item = m_ChannelStrips[row];
    return item->connectFrom(portName, side);
}

int ChannelStripModel::disconnectFrom(int row, const QString& portName, const int side) {
    ChannelStrip* item = m_ChannelStrips[row];
    return item->disconnectFrom(portName, side);
}

ChannelStrip* ChannelStripModel::getChannelStripByName(const QString channelStripName) const {

    for (int i = 0; i < m_ChannelStrips.length(); i++) {

        if (m_ChannelStrips[i]->getName() == channelStripName) {

            return m_ChannelStrips[i];
        }
    }
    return nullptr;
}

static int processChannelStrip (jack_nframes_t nframes, void *arg)
{
    ChannelStrip* m_ChannelStrip = (ChannelStrip*)arg;

    jack_default_audio_sample_t *in_1  = (jack_default_audio_sample_t*)jack_port_get_buffer (m_ChannelStrip->getInputPort1(),  nframes);
    jack_default_audio_sample_t *in_2  = (jack_default_audio_sample_t*)jack_port_get_buffer (m_ChannelStrip->getInputPort2(),  nframes);
    jack_default_audio_sample_t *out_1 = (jack_default_audio_sample_t*)jack_port_get_buffer (m_ChannelStrip->getOutputPort1(), nframes);
    jack_default_audio_sample_t *out_2 = (jack_default_audio_sample_t*)jack_port_get_buffer (m_ChannelStrip->getOutputPort2(), nframes);

    m_ChannelStrip->setPeakL(0.0f);
    m_ChannelStrip->setPeakR(0.0f);

    if (!m_ChannelStrip->getMute()) {
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
    QList<ChannelStrip*>* m_list = (QList<ChannelStrip*>*)arg;

    for (int i = m_list->length()-1; i >= 0; i--) {

        processChannelStrip (nframes, m_list->at(i));

    }

    return 0;
}


void ChannelStripModel::setCallback(jack_client_t *callBackJackClient) {
    jack_set_process_callback (callBackJackClient, processMixer, &m_ChannelStrips);
}


} // namespace Mixr
