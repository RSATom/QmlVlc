#ifndef QMLVLCAUDIO_H
#define QMLVLCAUDIO_H

#include <libvlc_wrapper/vlc_player.h>
#include <QObject>

class QmlVlcAudio : public QObject
{
    Q_OBJECT
public:
    QmlVlcAudio( vlc::player& player )
        : m_player( player ) { }

    Q_PROPERTY( unsigned trackCount READ get_trackCount );
    Q_PROPERTY( int track READ get_track WRITE set_track );
    Q_PROPERTY( bool mute READ get_mute WRITE set_mute );
    Q_PROPERTY( unsigned volume READ get_volume WRITE set_volume );
    Q_PROPERTY( unsigned channel READ get_channel WRITE set_channel );

    Q_PROPERTY( int libvlc_AudioChannel_Error MEMBER libvlc_AudioChannel_Error CONSTANT );
    Q_PROPERTY( int libvlc_AudioChannel_Stereo MEMBER libvlc_AudioChannel_Stereo CONSTANT );
    Q_PROPERTY( int libvlc_AudioChannel_RStereo MEMBER libvlc_AudioChannel_RStereo CONSTANT );
    Q_PROPERTY( int libvlc_AudioChannel_Left MEMBER libvlc_AudioChannel_Left CONSTANT );
    Q_PROPERTY( int libvlc_AudioChannel_Right MEMBER libvlc_AudioChannel_Right CONSTANT );
    Q_PROPERTY( int libvlc_AudioChannel_Dolbys MEMBER libvlc_AudioChannel_Dolbys CONSTANT );

    Q_PROPERTY( int stereo MEMBER libvlc_AudioChannel_Stereo CONSTANT );
    Q_PROPERTY( int reverseStereoeo MEMBER libvlc_AudioChannel_RStereo CONSTANT );
    Q_PROPERTY( int left MEMBER libvlc_AudioChannel_Left CONSTANT );
    Q_PROPERTY( int right MEMBER libvlc_AudioChannel_Right CONSTANT );
    Q_PROPERTY( int dolby MEMBER libvlc_AudioChannel_Dolbys CONSTANT );

    unsigned get_trackCount();

    bool get_mute();
    void set_mute( bool );

    unsigned get_volume();
    void set_volume( unsigned );

    int get_track();
    void set_track( int idx );

    unsigned get_channel();
    void set_channel( unsigned );

    void toggleMute();

    QString description( unsigned trackID );

private:
    vlc::player& m_player;
};

#endif //QMLVLCAUDIO_H
