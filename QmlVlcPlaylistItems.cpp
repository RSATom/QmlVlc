#include "QmlVlcPlaylistItems.h"

unsigned int QmlVlcPlaylistItems::get_count()
{
    return m_player.item_count();
}

void QmlVlcPlaylistItems::clear()
{
    return m_player.clear_items();
}

bool QmlVlcPlaylistItems::remove( unsigned int idx )
{
   return m_player.delete_item( idx );
}
