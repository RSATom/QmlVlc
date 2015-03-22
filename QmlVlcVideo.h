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

#ifndef QMLVLCVIDEO_H
#define QMLVLCVIDEO_H

#include "libvlc_wrapper/vlc_player.h"

#include "QmlVlcMarquee.h"
#include "QmlVlcLogo.h"
#include "QmlVlcDeinterlace.h"

class QmlVlcVideo : public QObject
{
    Q_OBJECT
public:
    QmlVlcVideo( vlc::player& player )
        : m_player( player ), m_marquee( player ), m_logo( player ), m_deinterlace( player ) { }

    Q_PROPERTY( unsigned width READ get_width )
    Q_PROPERTY( unsigned height READ get_height )
    Q_PROPERTY( unsigned count READ get_trackCount )

    Q_PROPERTY( int track READ get_track WRITE set_track )

    Q_PROPERTY( QString aspectRatio READ get_aspectRatio WRITE set_aspectRatio )

    Q_PROPERTY( QString crop READ get_crop WRITE set_crop )
    Q_PROPERTY( float contrast READ get_contrast WRITE set_contrast )
    Q_PROPERTY( float brightness READ get_brightness WRITE set_brightness )
    Q_PROPERTY( float hue READ get_hue WRITE set_hue )
    Q_PROPERTY( float saturation READ get_saturation WRITE set_saturation )
    Q_PROPERTY( float gamma READ get_gamma WRITE set_gamma )

    Q_PROPERTY( int teletext READ get_teletext WRITE set_teletext )

    Q_PROPERTY( QmlVlcMarquee* marquee READ get_marquee CONSTANT )
    Q_PROPERTY( QmlVlcLogo* logo READ get_logo CONSTANT )
    Q_PROPERTY( QmlVlcDeinterlace* deinterlace READ get_deinterlace CONSTANT )

    unsigned get_width();
    unsigned get_height();

    unsigned get_trackCount();

    int get_track();
    void set_track( int idx );

    QString get_aspectRatio();
    void set_aspectRatio( const QString& );

    QString get_crop();
    void set_crop( const QString& );

    int get_teletext();
    void set_teletext( unsigned );

    float get_contrast();
    void set_contrast( float v );

    float get_brightness();
    void set_brightness( float v );

    float get_hue();
    void set_hue( float v );

    float get_saturation();
    void set_saturation( float v );

    float get_gamma();
    void set_gamma( float v );

    Q_INVOKABLE void toggleTeletext();

    QmlVlcMarquee*     get_marquee()     { return &m_marquee; }
    QmlVlcLogo*        get_logo()        { return &m_logo; }
    QmlVlcDeinterlace* get_deinterlace() { return &m_deinterlace; }

private:
    void getVideoSize( unsigned* width, unsigned* height );

private:
    vlc::player& m_player;

    QmlVlcMarquee     m_marquee;
    QmlVlcLogo        m_logo;
    QmlVlcDeinterlace m_deinterlace;
};

#endif //QMLVLCVIDEO_H
