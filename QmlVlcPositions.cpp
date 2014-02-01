#include "QmlVlcPositions.h"

static const char* positions [] = {
    "center",        // 0
    "left",          // 1
    "right",         // 2
    0,               // 3
    "top",           // 4
    "top-left",      // 5
    "top-right",     // 6
    0,               // 7
    "bottom",        // 8
    "bottom-left",   // 9
    "bottom-right",  //10
};

QString QmlGetPositionById( unsigned int id )
{
    unsigned int p_size = sizeof( positions ) / sizeof( positions[0] );
    if( id < p_size && positions[id] ) {
        return positions[id];
    }
    else return QString();
}

int QmlGetIdByPosition( const QString& pos )
{
    int p_size = sizeof( positions ) / sizeof( positions[0] );
    for( int i = 0; i < p_size; ++i ) {
        if( positions[i] && pos == positions[i] )
            return i;
    }
    return -1;
}
