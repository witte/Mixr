#ifndef CHANNELSTRIPMODEL_H
#define CHANNELSTRIPMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QList>

#include "ChannelStrip.h"

namespace Mixr {

class ChannelStripModel : public QAbstractListModel
{
    Q_OBJECT


public:
    explicit ChannelStripModel(jack_client_t* jackClientName, QObject* parent = 0);
    ~ChannelStripModel();

    enum Roles  {
        NameRole = Qt::UserRole + 0,
        PanRole = Qt::UserRole + 1,
        VolumeRole = Qt::UserRole + 2,
        TempMuteRole = Qt::UserRole + 3,
        MuteRole = Qt::UserRole + 4,
        SoloRole = Qt::UserRole + 5,
        ParentRole = Qt::UserRole + 6,
        LevelRole = Qt::UserRole + 7, // 'Level' in the hierarchy
        ColorRole = Qt::UserRole + 8,
        ParentColorsRole = Qt::UserRole + 9,
        VisibleRole = Qt::UserRole + 10,
        Widgets = Qt::UserRole + 11,
        ChildrenRole = Qt::UserRole + 12
    };
    Q_ENUM(Roles)


    Q_INVOKABLE bool setPan   (int row, const float &value);
    Q_INVOKABLE bool setVolume(int row, const float &value);
    Q_INVOKABLE bool setMute  (int row, const bool  &value);
    Q_INVOKABLE bool setColor (int row, const char  &value);
    Q_INVOKABLE QStringList getJackOutputPorts();
    Q_INVOKABLE int connectFrom(int row, const QString& portName, const int side);
    Q_INVOKABLE int disconnectFrom(int row, const QString& portName, const int side);

    ChannelStrip* getChannelStripByName(const QString channelStripName) const;

//    void add(ChannelStrip* channelStrip, const QString channelStripParentName, QObject* csParent);
    void add(const QString channelStripName, const QString channelStripParentName, QObject* csParent);

    void setCallback(jack_client_t* callBackJackClient);
    jack_client_t* jackClient;


protected:
    QHash<int,QByteArray> roleNames() const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;


    int rowCount() const Q_DECL_OVERRIDE;


private:
    QList<ChannelStrip*> m_ChannelStrips;


};

}

//Q_DECLARE_METATYPE(Mixr::ChannelStripModel)

#endif // CHANNELSTRIPMODEL_H
