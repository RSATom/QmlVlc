#include "QmlVlcVideoFrame.h"

QmlVlcI420Frame::QmlVlcI420Frame()
    : width( 0 ), height( 0 ),
      yPlane( 0 ), yPlaneSize( 0 ),
      uPlane( 0 ), uPlaneSize( 0 ),
      vPlane( 0 ), vPlaneSize( 0 )
{
}

void QmlVlcI420Frame::clear()
{
    width = height = 0;
    yPlane = uPlane = vPlane = 0;
    yPlaneSize = uPlaneSize = vPlaneSize = 0;
}

bool QmlVlcI420Frame::isValid()
{
    return width > 0 && height > 0;
}
