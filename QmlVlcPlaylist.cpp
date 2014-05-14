#include "QmlVlcPlaylist.h"

#include <QStringList>

#include "QmlVlcConfig.h"

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

int QmlVlcPlaylist::add( const QString& mrl )
{
    return m_player.add_media( mrl.toUtf8().data() );
}

int QmlVlcPlaylist::addWithOptions( const QString& mrl, const QStringList& options )
{
    QList<QByteArray> bufStorage;
    std::vector<const char*> trusted_opts;
    std::vector<const char*> untrusted_opts;

    for( int i = 0; i < options.size(); ++i ) {
        QByteArray& buf = *bufStorage.insert( bufStorage.end(), options[i].toUtf8() );
        if( QmlVlcConfig::trustedEnvironment() )
            trusted_opts.push_back( buf.data() );
        else
            untrusted_opts.push_back( buf.data() );
    }

    const char** untrusted_optv = untrusted_opts.empty() ? 0 : &untrusted_opts[0];
    const char** trusted_optv = trusted_opts.empty() ? 0 : &trusted_opts[0];
    return m_player.add_media( mrl.toUtf8().data(),
                               untrusted_opts.size(), untrusted_optv,
                               trusted_opts.size(), trusted_optv );
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
