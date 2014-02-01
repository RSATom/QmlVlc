#ifndef QMLVLCDEINTERLACE_H
#define QMLVLCDEINTERLACE_H

#include <libvlc_wrapper/vlc_player.h>

#include <QObject.h>

class QmlVlcDeinterlace : public QObject
{
    Q_OBJECT
public:
    QmlVlcDeinterlace( vlc::player& player )
        : m_player( player ) { }

    Q_INVOKABLE void enable( const QString& mode );
    Q_INVOKABLE void disable();

private:
    vlc::player& m_player;
};

#endif //QMLVLCDEINTERLACE_H
