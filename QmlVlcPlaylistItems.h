#ifndef PLAYLISTITEMS_H
#define PLAYLISTITEMS_H

#include <libvlc_wrapper/vlc_player.h>

class QmlVlcPlaylistItems : public QObject
{
    Q_OBJECT
public:
    QmlVlcPlaylistItems( vlc::player& player )
        : m_player( player ) { }

    Q_PROPERTY( unsigned count READ get_count );

    unsigned get_count();

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool remove( unsigned idx );

private:
    vlc::player& m_player;
};

#endif //PLAYLISTITEMS_H

