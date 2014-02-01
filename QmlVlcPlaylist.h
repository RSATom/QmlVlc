#ifndef QMLVLCPLAYLIST_H
#define QMLVLCPLAYLIST_H

#include <libvlc_wrapper/vlc_player.h>

#include "QmlVlcPlaylistItems.h"

class QmlVlcPlaylist : public QObject
{
    Q_OBJECT
public:
    QmlVlcPlaylist( vlc::player& player )
        : m_player( player ), m_items( player ) { }

    Q_PROPERTY( unsigned itemCount READ get_itemCount );
    Q_PROPERTY( bool isPlaying READ get_isPlaying );
    Q_PROPERTY( int currentItem READ get_current );

    Q_PROPERTY( QmlVlcPlaylistItems* items READ get_items CONSTANT );

    unsigned get_itemCount();
    bool get_isPlaying();
    int get_current();

    Q_INVOKABLE void play();
    Q_INVOKABLE bool playItem( unsigned idx );
    Q_INVOKABLE void setCurrentItem( unsigned idx );
    Q_INVOKABLE void pause();
    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void next();
    Q_INVOKABLE void prev();
    Q_INVOKABLE void clear();
    Q_INVOKABLE bool removeItem( unsigned idx );

    QmlVlcPlaylistItems* get_items() { return &m_items; }

private:
    vlc::player& m_player;

    QmlVlcPlaylistItems m_items;
};

#endif //QMLVLCPLAYLIST_H
