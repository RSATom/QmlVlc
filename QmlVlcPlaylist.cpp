#include "QmlVlcPlaylist.h"

unsigned int QmlVlcPlaylist::get_itemCount()
{
    return m_player.item_count();
}

bool QmlVlcPlaylist::get_isPlaying()
{
    return m_player.is_playing();
}

int QmlVlcPlaylist::get_current()
{
    return m_player.current_item();
}

void QmlVlcPlaylist::play()
{
    m_player.play();
}

bool QmlVlcPlaylist::playItem( unsigned int idx )
{
    return m_player.play( idx );
}

void QmlVlcPlaylist::setCurrentItem( unsigned int idx )
{
    m_player.set_current( idx );
}

void QmlVlcPlaylist::pause()
{
    m_player.pause();
}

void QmlVlcPlaylist::togglePause()
{
    if ( m_player.is_playing() ) m_player.pause();
    else m_player.play();
}

void QmlVlcPlaylist::stop()
{
    //use async stop to avoid freeze (on network timeout for example).
    m_player.stop( true );
}

void QmlVlcPlaylist::next()
{
    m_player.next();
}

void QmlVlcPlaylist::prev()
{
    m_player.prev();
}

void QmlVlcPlaylist::clear()
{
    m_player.clear_items();
}

bool QmlVlcPlaylist::removeItem( unsigned int idx )
{
    return m_player.delete_item( idx );
}
