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

#include "QmlVlcInput.h"

double QmlVlcInput::get_length()
{
    return static_cast<double>( m_player.get_length() );
}

double QmlVlcInput::get_fps()
{
    return m_player.get_fps();
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
