#ifndef RENDERSETWIDGET_H
#define RENDERSETWIDGET_H

#include "glbase.h"
#include "RenderSet.h"
#include "FrameCounter.h"
#include <QGLWidget>
#include <QPointF>
#include <QPoint>

//=============================================================================
//  RenderSetWidget
//=============================================================================

class QMouseEvent;
class QWheelEvent;
class RenderSet;
class QTimer;
class QAction;

/**
    \class RenderSetWidget

    - Responsible for visualization and editing of a RenderSet.
    - Does *not* own the RenderSet but only a pointer to one.
*/
class RenderSetWidget : public QGLWidget
{
    Q_OBJECT

signals:
    void closed();

public:
    enum InteractionState { 
        EditVertexState,
        PickedVertexState, 
        PaintMaskState
    };

    enum RenderFlags {
        RenderPreview = 1,
        RenderFinal   = 2,
        RenderGrid    = 4
    };
    
    /// C'tor
    RenderSetWidget( QWidget *parent, QGLWidget *share );
    ~RenderSetWidget() { emit closed(); }

    /// Set RenderSet, pointer must stay valid while it is assigned here!
    void setRenderSet( RenderSet* set ) { m_set = set; }

    void setRenderUpdateEnabled( bool b );

    QList<QAction*> getRenderActions() { return renderActions; }

    ///@name Fullscreen
    ///@{
    QAction* getToggleFullscreenAction() { return toggleFullscreenAction; }
public slots:
    void toggleFullscreen( bool enabled );
    bool isFullscreen() const { return m_fullscreen; }
    ///@}

    void showContextMenu( const QPoint& );

protected:
    ///@name QGLWidget implementation
    ///@{ 
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
    QTimer* m_renderUpdateTimer;
    ///@}

    ///@name Mouse events
    ///@{
    QPointF normalizedCoordinates( QPointF pos );
    virtual void mousePressEvent  ( QMouseEvent* e ) override;
    virtual void mouseReleaseEvent( QMouseEvent* e ) override;
    virtual void mouseMoveEvent   ( QMouseEvent* e ) override;
    virtual void wheelEvent( QWheelEvent* e ) override;
    ///@}

private:
    RenderSet* m_set;
    int        m_state;
    int        m_flags;
    QPointF    m_delta;
    bool       m_fullscreen;
    FrameCounter m_fps;
    int        m_maskRadius;
    QPointF    m_cursorPos;
    int        m_rotScreen;

private:
    QAction* toggleFullscreenAction;
    QList<QAction*> maskActions;
    QList<QAction*> renderActions;
};

#endif // RENDERSETWIDGET_H
