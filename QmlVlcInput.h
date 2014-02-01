#ifndef QMLVLCINPUT_H
#define QMLVLCINPUT_H

#include <libvlc_wrapper/vlc_player.h>

class QmlVlcInput : public QObject
{
    Q_OBJECT
public:
    QmlVlcInput( vlc::player& player )
        : m_player( player ) { }

    Q_PROPERTY( double length READ get_length() )
    Q_PROPERTY( double fps READ get_fps() )
    Q_PROPERTY( bool hasVout READ get_hasVout() )
    Q_PROPERTY( unsigned state READ get_state() )
    Q_PROPERTY( double position READ get_position() )
    Q_PROPERTY( double time READ get_time() )
    Q_PROPERTY( double rate READ get_rate() )

    double get_length();
    double get_fps();
    bool get_hasVout();
    unsigned get_state();

    double get_position();
    void set_position( double );

    double get_time();
    void set_time( double );

    double get_rate();
    void set_rate( double );

private:
    vlc::player& m_player;
};

#endif //QMLVLCINPUT_H
