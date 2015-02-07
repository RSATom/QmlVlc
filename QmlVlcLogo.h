/*******************************************************************************
* Copyright © 2014, Sergey Radionov <rsatom_gmail.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.

* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#ifndef QMLVLCLOGO_H
#define QMLVLCLOGO_H

#include <QObject>

#include "libvlc_wrapper/vlc_player.h"

class QmlVlcLogo : public QObject
{
    Q_OBJECT
public:
    QmlVlcLogo( vlc::player& player )
        : m_player( player ) { }

    Q_PROPERTY( QString position READ get_position WRITE set_position )
    Q_PROPERTY( unsigned opacity READ get_opacity WRITE set_opacity )
    Q_PROPERTY( unsigned delay READ get_delay WRITE set_delay )
    Q_PROPERTY( int repeat READ get_repeat WRITE set_repeat )
    Q_PROPERTY( unsigned x READ get_x WRITE set_x )
    Q_PROPERTY( unsigned y READ get_y WRITE set_y )

    QString get_position();
    void set_position( const QString& );

    unsigned get_opacity()
        { return get_logo_int( libvlc_logo_opacity ); }
    void set_opacity( unsigned o )
        { set_logo_int( libvlc_logo_opacity, o ); }

    unsigned get_delay()
        { return get_logo_int( libvlc_logo_delay ); }
    void set_delay( unsigned d )
        { set_logo_int( libvlc_logo_delay, d ); }

    int get_repeat()
        { return get_logo_int( libvlc_logo_repeat ); }
    void set_repeat( int r )
        { set_logo_int( libvlc_logo_repeat, r ); }

    unsigned get_x()
        { return get_logo_int( libvlc_logo_x ); }
    void set_x( unsigned x )
        { set_logo_int( libvlc_logo_x, x ); }

    unsigned int get_y()
        { return get_logo_int( libvlc_logo_y ); }
    void set_y( unsigned y )
        { set_logo_int( libvlc_logo_y, y ); }

    Q_INVOKABLE void enable()
        { return set_logo_int( libvlc_logo_enable, 1 ); }
    Q_INVOKABLE void disable()
        { return set_logo_int( libvlc_logo_enable, 0 ); }

    Q_INVOKABLE void file( const QString& );

private:
    int get_logo_int( libvlc_video_logo_option_t );
    void set_logo_int( libvlc_video_logo_option_t, int i );

private:
    vlc::player& m_player;
};

#endif //QMLVLCLOGO_H
