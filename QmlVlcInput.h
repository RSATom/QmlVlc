/*******************************************************************************
* Copyright © 2014-2016, Sergey Radionov <rsatom_gmail.com>
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

#pragma once

#include <QObject>
#include "qmlvlclib_global.h"
#include "libvlc_wrapper/vlc_player.h"

class QMLVLCLIB_EXPORT QmlVlcInput : public QObject
{
    Q_OBJECT
public:
    QmlVlcInput( vlc::player_core& player )
        : m_player( player ) { }

    Q_PROPERTY( double length READ get_length )
    Q_PROPERTY( double fps READ get_fps )
    Q_PROPERTY( bool hasVout READ get_hasVout )
    Q_PROPERTY( unsigned state READ get_state )

    Q_PROPERTY( double position READ get_position WRITE set_position )
    Q_PROPERTY( double time READ get_time WRITE set_time )
    Q_PROPERTY( double rate READ get_rate WRITE set_rate )

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


    Q_PROPERTY( unsigned titleCount READ get_titleCount )
    Q_PROPERTY( int title READ get_title WRITE set_title )

    unsigned get_titleCount();

    int get_title();
    void set_title( unsigned );


    Q_PROPERTY( unsigned chapterCount READ get_chapterCount )
    Q_PROPERTY( int chapter READ get_chapter WRITE set_chapter )

    unsigned get_chapterCount();

    int get_chapter();
    void set_chapter( unsigned );

    Q_INVOKABLE void prevChapter();
    Q_INVOKABLE void nextChapter();

private:
    vlc::player_core& m_player;
};
