#include "QmlVlcInput.h"

double QmlVlcInput::get_length()
{
    return static_cast<double>( m_player.current_media().get_length() );
}

double QmlVlcInput::get_fps()
{
    return m_player.current_media().get_fps();
}

bool QmlVlcInput::get_hasVout()
{
    return m_player.video().has_vout();
}

unsigned int QmlVlcInput::get_state()
{
    return m_player.get_state();
}

double QmlVlcInput::get_position()
{
    return m_player.get_position();
}

void QmlVlcInput::set_position( double pos )
{
    m_player.set_position( static_cast<float>( pos ) );
}

double QmlVlcInput::get_time()
{
    return static_cast<double>( m_player.get_time() );
}

void QmlVlcInput::set_time( double t )
{
    return m_player.set_time( static_cast<libvlc_time_t>( t ) );
}

double QmlVlcInput::get_rate()
{
    return m_player.get_rate();
}

void QmlVlcInput::set_rate( double r )
{
    m_player.set_rate( static_cast<float>( r ) );
}
