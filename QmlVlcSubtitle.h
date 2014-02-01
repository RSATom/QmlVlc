#ifndef SUBTITLE_H
#define SUBTITLE_H

#include <libvlc_wrapper/vlc_player.h>

class QmlVlcSubtitle : public QObject
{
    Q_OBJECT
public:
    QmlVlcSubtitle( vlc::player& player )
        : m_player( player ) { }

    Q_PROPERTY( unsigned trackCount READ get_trackCount );

    Q_PROPERTY( int track READ get_track WRITE set_track );
    unsigned get_trackCount();

    int get_track();
    void set_track( int idx );

    QString description( unsigned i );

private:
    vlc::player& m_player;
};

#endif //SUBTITLE_H
