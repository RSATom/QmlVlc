#ifndef QMLVLCLOGO_H
#define QMLVLCLOGO_H

#include <libvlc_wrapper/vlc_player.h>

class QmlVlcLogo : public QObject
{
    Q_OBJECT
public:
    QmlVlcLogo( vlc::player& player )
        : m_player( player ) { }

    Q_PROPERTY( QString position READ get_position WRITE set_position );
    Q_PROPERTY( unsigned opacity READ get_opacity WRITE set_opacity );
    Q_PROPERTY( unsigned delay READ get_delay WRITE set_delay );
    Q_PROPERTY( int repeat READ get_repeat WRITE set_repeat );
    Q_PROPERTY( unsigned x READ get_x WRITE set_x );
    Q_PROPERTY( unsigned y READ get_y WRITE set_y );

    QString get_position();
    void set_position( const QString& );

    unsigned get_opacity()
        { return get_logo_int( libvlc_logo_opacity ); };
    void set_opacity( unsigned o )
        { set_logo_int( libvlc_logo_opacity, o ); };

    unsigned get_delay()
        { return get_logo_int( libvlc_logo_delay ); };
    void set_delay( unsigned d )
        { set_logo_int( libvlc_logo_delay, d ); };

    int get_repeat()
        { return get_logo_int( libvlc_logo_repeat ); };
    void set_repeat( int r )
        { set_logo_int( libvlc_logo_repeat, r ); };

    unsigned get_x()
        { return get_logo_int( libvlc_logo_x ); };
    void set_x( unsigned x )
        { set_logo_int( libvlc_logo_x, x ); };

    unsigned int get_y()
        { return get_logo_int( libvlc_logo_y ); };
    void set_y( unsigned y )
        { set_logo_int( libvlc_logo_y, y ); };

    void enable()
        { return set_logo_int( libvlc_logo_enable, 1 ); }
    void disable()
        { return set_logo_int( libvlc_logo_enable, 0 ); }

    void file( const QString& );

private:
    int get_logo_int( libvlc_video_logo_option_t );
    void set_logo_int( libvlc_video_logo_option_t, int i );

private:
    vlc::player& m_player;
};

#endif //QMLVLCLOGO_H
