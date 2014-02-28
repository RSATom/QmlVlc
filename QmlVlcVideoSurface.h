#ifndef QMLVLCVIDEOSURFACE_H
#define QMLVLCVIDEOSURFACE_H

#include <QQuickItem>
#include <QPointer>

#include "QmlVlcVideoOutput.h"

struct QmlVlcI420Frame;//#include "QmlVlcVideoFrame.h"

class QmlVlcVideoSurface
    : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QmlVlcVideoOutput* source READ source WRITE setSource NOTIFY sourceChanged )

public:
    QmlVlcVideoSurface();
    ~QmlVlcVideoSurface();

    QmlVlcVideoOutput* source() const
        { return m_source.data(); }
    void setSource( QmlVlcVideoOutput* source );

    virtual QSGNode* updatePaintNode( QSGNode*, UpdatePaintNodeData* );

public Q_SLOTS:
    void presentFrame( const QSharedPointer<const QmlVlcI420Frame>& frame );

Q_SIGNALS:
    void sourceChanged();

private:
    QPointer<QmlVlcVideoOutput> m_source;

    QSharedPointer<const QmlVlcI420Frame> m_frame;
};

#endif //QMLVLCVIDEOSURFACE_H
