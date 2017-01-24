#include "ChannelStripModel.h"
#include "ChannelStrip.h"

#include "mixer.h"

namespace Mixr {


ChannelStripModel::ChannelStripModel(jack_client_t* jackClientName, QObject *parent) : QAbstractListModel(parent)
{
    jackClient = jackClientName;
}

ChannelStripModel::~ChannelStripModel() { }

QHash<int,QByteArray> ChannelStripModel::roleNames() const // Q_DECL_OVERRIDE
{
     QHash<int, QByteArray> result;
     result.insert(NameRole,     QByteArrayLiteral("csName"));
     result.insert(PanRole,      QByteArrayLiteral("csPan"));
     result.insert(VolumeRole,   QByteArrayLiteral("csVolume"));
     result.insert(TempMuteRole, QByteArrayLiteral("csTempMute"));
     result.insert(MuteRole,     QByteArrayLiteral("csMute"));
     result.insert(SoloRole,     QByteArrayLiteral("csSolo"));
     result.insert(ParentRole,   QByteArrayLiteral("csParent"));
     result.insert(LevelRole,    QByteArrayLiteral("csLevel"));
     result.insert(ColorRole,    QByteArrayLiteral("csColor"));
     result.insert(VisibleRole,  QByteArrayLiteral("csVisible"));
     result.insert(Widgets,      QByteArrayLiteral("csWidgets"));
     result.insert(ChildrenRole, QByteArrayLiteral("csChildren"));
     return result;
}

QVariant ChannelStripModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const //Q_DECL_OVERRIDE
{
    if (!index.isValid())
        return QVariant();

    const ChannelStrip* item = m_ChannelStrips[index.row()];

    switch (role) {
        case NameRole:      return QVariant::fromValue(item->getName()); break;
        case PanRole:       return QVariant::fromValue(item->getPan()); break;
        case VolumeRole:    return QVariant::fromValue(item->getVolume()); break;
        case TempMuteRole:  return QVariant::fromValue(item->getTempMute()); break;
        case MuteRole:      return QVariant::fromValue(item->getMute()); break;
        case SoloRole:      return QVariant::fromValue(item->getSolo()); break;
        case ParentRole:
        case LevelRole:
        case ColorRole:
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
            case ParentRole:
            case LevelRole:
            case ColorRole:
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

        // We'll use this to get information from the parent channels, to make their children look
        // indented even if they're not a hierarchy in the model

//        ChannelStrip *item = m_ChannelStrips[row];
//        if (item->getParent() != nullptr) {

//            int m_parentIndex = m_ChannelStrips.indexOf(item->getParent());
//            setData(createIndex(m_parentIndex, 0), value, PanRole);
//        }

        return true;
    }
    else return false;
}


int ChannelStripModel::rowCount() const {
    return m_ChannelStrips.length();
}

void ChannelStripModel::add(ChannelStrip* channelStrip, ChannelStrip* channelStripParent) {

    if (channelStripParent != nullptr)
        channelStrip->setParent(channelStripParent);

//    m_ChannelStrips.insert(0, channelStrip);
}

void ChannelStripModel::add(QString channelStripName, QString channelStripParentName) {

    Mixr::ChannelStrip* m_Channel = new ChannelStrip(jackClient, channelStripName);

    m_ChannelStrips.append(m_Channel);

    if (channelStripParentName == "") {
//        qDebug("Setting %s as parent of %s", m_ChannelStrips.first()->getName().toLatin1().data(),
//               m_Channel->getName().toLatin1().data());
        m_Channel->setParent( m_ChannelStrips.first() );
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

    for (int i = 0; i < m_list->length(); i++) {

        processChannelStrip (nframes, m_list->at(i));

    }

    return 0;
}


void ChannelStripModel::setCallback(jack_client_t *callBackJackClient) {
    jack_set_process_callback (callBackJackClient, processMixer, &m_ChannelStrips);
}


} // namespace Mixr
