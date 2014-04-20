#ifndef QMLVLCAUDIO_H
#define QMLVLCAUDIO_H

#include <QObject>

#include "libvlc_wrapper/vlc_player.h"

class QmlVlcAudio : public QObject
{
    Q_OBJECT
public:
    QmlVlcAudio( vlc::player& player )
        : m_player( player ) { }

    enum Output {
        Stereo = ::libvlc_AudioChannel_Stereo,
        ReverseStereo = ::libvlc_AudioChannel_RStereo,
        Left = ::libvlc_AudioChannel_Left,
        Right = ::libvlc_AudioChannel_Right,
        Dolby = ::libvlc_AudioChannel_Dolbys,
    };
    Q_ENUMS( Output )

    Q_PROPERTY( unsigned trackCount READ get_trackCount )
    Q_PROPERTY( int track READ get_track WRITE set_track )
    Q_PROPERTY( bool mute READ get_mute WRITE set_mute )
    Q_PROPERTY( unsigned volume READ get_volume WRITE set_volume )
    Q_PROPERTY( Output channel READ get_channel WRITE set_channel )

    unsigned get_trackCount();

    bool get_mute();
    void set_mute( bool );

    unsigned get_volume();
    void set_volume( unsigned );

    int get_track();
    void set_track( int idx );

    Output get_channel();
    void set_channel( Output );

    void toggleMute();

    QString description( unsigned trackID );

private:
    vlc::player& m_player;
};

#endif //QMLVLCAUDIO_H
