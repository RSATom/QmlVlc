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

#ifndef QMLVLCMARQUEE_H
#define QMLVLCMARQUEE_H

#include <QObject>

#include "libvlc_wrapper/vlc_player.h"

class QmlVlcMarquee : public QObject
{
    Q_OBJECT
public:
    QmlVlcMarquee( vlc::player& player )
        : m_player( player ) { }

    Q_PROPERTY( QString text READ get_text WRITE set_text )
    Q_PROPERTY( unsigned color READ get_color WRITE set_color )
    Q_PROPERTY( unsigned opacity READ get_opacity WRITE set_opacity )
    Q_PROPERTY( QString position READ get_position WRITE set_position )
    Q_PROPERTY( unsigned refresh READ get_refresh WRITE set_refresh )
    Q_PROPERTY( unsigned size READ get_size WRITE set_size )
    Q_PROPERTY( unsigned timeout READ get_timeout WRITE set_timeout )
    Q_PROPERTY( unsigned x READ get_x WRITE set_x )
    Q_PROPERTY( unsigned y READ get_y WRITE set_y )

    QString get_text();
    void set_text( const QString& t );

    unsigned get_color()
        {  return get_marquee_int( libvlc_marquee_Color ); }
    void set_color( unsigned c )
        {  set_marquee_int( libvlc_marquee_Color, c ); }

    unsigned get_opacity()
        {  return get_marquee_int( libvlc_marquee_Opacity ); }
    void set_opacity( unsigned o )
        {  set_marquee_int( libvlc_marquee_Opacity, o ); }

    QString get_position();
    void set_position( const QString& p );

    unsigned get_refresh()
        {  return get_marquee_int( libvlc_marquee_Refresh ); }
    void set_refresh( unsigned r )
        {  set_marquee_int( libvlc_marquee_Refresh, r ); }

    unsigned get_size()
        {  return get_marquee_int( libvlc_marquee_Size ); }
    void set_size( unsigned s )
        {  set_marquee_int( libvlc_marquee_Size, s ); }

    unsigned get_timeout()
        {  return get_marquee_int( libvlc_marquee_Timeout ); }
    void set_timeout( unsigned t )
        {  set_marquee_int( libvlc_marquee_Timeout, t ); }

    unsigned get_x()
        {  return get_marquee_int( libvlc_marquee_X ); }
    void set_x( unsigned x )
        {  set_marquee_int( libvlc_marquee_X, x ); }

    unsigned get_y()
        {  return get_marquee_int( libvlc_marquee_Y ); }
    void set_y( unsigned y )
        {  set_marquee_int( libvlc_marquee_Y, y ); }

    Q_INVOKABLE void enable()
        { set_marquee_int( libvlc_marquee_Enable, 1 ); }
    Q_INVOKABLE void disable()
        { set_marquee_int( libvlc_marquee_Enable, 0 ); }

private:
    int get_marquee_int( libvlc_video_marquee_option_t );
    void set_marquee_int( libvlc_video_marquee_option_t, int i );

private:
    vlc::player& m_player;
};

#endif //QMLVLCMARQUEE_H
