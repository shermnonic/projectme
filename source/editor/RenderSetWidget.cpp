#include "RenderSetWidget.h"
#include "glbase.h"
#include <iostream>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QPoint>
#include <QMessageBox>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QDebug>

//=============================================================================
//  RenderSetWidget
//=============================================================================

//-----------------------------------------------------------------------------
RenderSetWidget::RenderSetWidget( QWidget *parent, QGLWidget *share )
: QGLWidget( parent, share ),  
  m_set( 0 ),  
  m_state( EditVertexState ),
  m_flags( RenderPreview ),
  m_fullscreen( false ),
  m_maskRadius( 23 ),
  m_cursorPos( 0.,0. ),
  m_rotScreen( 0 )
{
    // Context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &RenderSetWidget::showContextMenu);

    // Actions
    toggleFullscreenAction = new QAction(tr("Toggle fullscreen"),this);
    toggleFullscreenAction->setCheckable(true);
    toggleFullscreenAction->setChecked(m_fullscreen);
    connect(toggleFullscreenAction, &QAction::toggled, this, &RenderSetWidget::toggleFullscreen);

    {
        QAction* maskEditAction = new QAction(tr("Mask edit"), this);
        maskEditAction->setCheckable(true);
        maskEditAction->setChecked(m_state == PaintMaskState);
        connect(maskEditAction, &QAction::toggled, this, [=](bool checked){ m_state = checked ? PaintMaskState : EditVertexState; });

        QAction* maskClearWhiteAction = new QAction(tr("Mask clear white"), this);
        connect(maskClearWhiteAction, &QAction::triggered, this, [=](){ m_set->clearMask( true ); });

        QAction* maskClearBlackAction = new QAction(tr("Mask clear black"), this);
        connect(maskClearBlackAction, &QAction::triggered, this, [=](){ m_set->clearMask( false ); });

        QAction* maskLoadAction = new QAction(tr("Load mask"), this);
        connect(maskLoadAction, &QAction::triggered, this, [=]()
        { 
            QString filename = QFileDialog::getOpenFileName( this,
                tr("Open mask image"), QString(), tr("Images (*.png *.jpg)") );

            if(!filename.isEmpty())
            {
                if(!m_set->loadMask(filename.toStdString().c_str()))
                {
                    qDebug() << "Could not load " << filename << "!";
                }
            }
        });

        QAction* maskSaveAction = new QAction(tr("Save mask"), this);
        connect(maskSaveAction, &QAction::triggered, this, [=]()
        {
            QString filename = QFileDialog::getSaveFileName( this,
                tr("Save mask image"), QString(), tr("Images (*.png *.jpg)") );

            if(!filename.isEmpty())
            {
                if(!m_set->saveMask( filename.toStdString().c_str()))
                {
                    qDebug() << "Could not save " << filename << "!";
                }
            }
        });

        maskActions.push_back(maskEditAction);
        maskActions.push_back(maskClearWhiteAction);
        maskActions.push_back(maskClearBlackAction);
        maskActions.push_back(maskLoadAction);
        maskActions.push_back(maskSaveAction);
    }
    {
        QAction* renderPreviewAction = new QAction(tr("Render preview"), this);
        QAction* renderFinalAction = new QAction(tr("Render final"), this);
        QAction* renderGridAction = new QAction(tr("Render grid"), this);
        renderPreviewAction->setCheckable(true);
        renderFinalAction->setCheckable(true);
        renderGridAction->setCheckable(true);

        auto renderGroup = new QActionGroup(this);
        renderGroup->addAction(renderPreviewAction);
        renderGroup->addAction(renderFinalAction);
        renderGroup->addAction(renderGridAction);
        renderPreviewAction->setChecked(true);
        
        connect(renderPreviewAction, &QAction::triggered, this, [=]() { m_flags = RenderPreview; });
        connect(renderFinalAction, &QAction::triggered, this, [=]() { m_flags = RenderFinal; });
        connect(renderGridAction, &QAction::triggered, this, [=]() { m_flags = RenderFinal | RenderGrid; });

        renderActions.push_back(renderPreviewAction);
        renderActions.push_back(renderFinalAction);
        renderActions.push_back(renderGridAction);
    }


    // Render update timer
    m_renderUpdateTimer = new QTimer(this);
    connect(m_renderUpdateTimer, &QTimer::timeout, this, &RenderSetWidget::updateGL);
    m_renderUpdateTimer->start(16); // 16ms=60fps, 42ms=24fps

    // DEBUG
    qDebug() << "The newly created RenderSetWidget" << 
        (isSharing() ? "is sharing!" : "is *not* sharing?!");

    setMouseTracking(true);
}

//-----------------------------------------------------------------------------
void RenderSetWidget::setRenderUpdateEnabled( bool b )
{
    if (!b)
        m_renderUpdateTimer->stop();
    else
        if (!m_renderUpdateTimer->isActive())
            m_renderUpdateTimer->start(42);
}

//-----------------------------------------------------------------------------
void RenderSetWidget::initializeGL()
{
    // The context should already be initialized correctly by the master
    // QGLWidget whose context this RenderSetWidget shares.
}

//-----------------------------------------------------------------------------
void RenderSetWidget::resizeGL( int w, int h )
{
    //if( m_rotScreen % 2 == 1 )
    //  std::swap( w, h );

    // Set some default projection matrix. 
    // Note that this may be overidden in the render() call!
    float aspect = (float)w/h,
          fov    = (float)45.f,
          znear  =   0.1f,
          zfar   =  42.0f;

    glViewport( 0,0, w,h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( fov, aspect, znear, zfar );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    if( m_rotScreen > 0 )
        glRotatef( (GLfloat)m_rotScreen*90, (GLfloat)0,(GLfloat)0,(GLfloat)1 );
}

//-----------------------------------------------------------------------------
void RenderSetWidget::paintGL()
{   
    glClearColor( 0,0,0,1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( m_set )
    {
        if( m_flags & RenderFinal )
        {
            if( m_flags & RenderGrid )
            {
                GLuint texid = 
                    bindTexture( QPixmap(":/grid.png"), GL_TEXTURE_2D, GL_RGBA );
                m_set->render( texid );
            }
            else
                m_set->render();
        }

        if( m_flags & RenderPreview )
        {
            m_set->drawMask();
            m_set->drawOutline();
        }

        if (m_state == PaintMaskState)
        {
            m_set->drawMaskMarker(m_cursorPos.x(), m_cursorPos.y(), m_maskRadius / 1024.f, m_maskRadius / 768.f);
        }
    }
    
    float fps = m_fps.measure();
    setWindowTitle(tr("Renderer - %1 FPS").arg((unsigned)fps));
}

//-----------------------------------------------------------------------------
void RenderSetWidget::toggleFullscreen( bool enabled )
{
#if 0
    setWindowFlags( Qt::Window | (b ? Qt::FramelessWindowHint : Qt::Window) );
    move( 1280, 0 ); // FIXME: Hardcoded screen position
    show();
#else
    setWindowState( windowState() ^ Qt::WindowFullScreen );
#endif  
    m_fullscreen = enabled;
}

//-----------------------------------------------------------------------------
QPointF RenderSetWidget::normalizedCoordinates( QPointF pos )
{
    float x = pos.x() / width(),
          y = (height()-pos.y()) / height(); // invert y-axis

    QPointF pt( 2.f*x - 1.f, 2.f*y - 1.f );

    if( m_rotScreen==1 ) // 90°
        pt = QPointF(pt.y(),2.-(pt.x()+1.)-1.);
    else if( m_rotScreen==2 ) // 180°
        pt = QPointF(-pt.x(),-pt.y());
    else if( m_rotScreen==3 ) // 270°
        pt = QPointF(-pt.y(),pt.x());

    return pt;
}

//-----------------------------------------------------------------------------
void RenderSetWidget::mousePressEvent( QMouseEvent* e )
{       
    QGLWidget::mouseMoveEvent( e );
    if( e->isAccepted() )
        return;

    if( !m_set ) 
        return;

    // Normalized coordinates in [-1,-1]-[1,1]
    QPointF pt = normalizedCoordinates( e->localPos() );
    m_cursorPos = pt;
    
    if( m_state == EditVertexState )
    {
        // Handle area editing interaction
        if( e->buttons() & Qt::LeftButton )
        {
            if( m_set->pickVertex( pt.x(), pt.y() ) >= 0 )
            {
                m_state = PickedVertexState;
                
                // Delta between mouse and vertex position for translation
                float x,y;
                m_set->getPickedVertexPosition( x, y );
                m_delta = pt - QPointF( x, y );
            }
            else
                m_state = EditVertexState;

            e->accept();
        }
    }
    else if( m_state == PaintMaskState )
    {
        // Handle mask painting
        if( e->buttons() & Qt::LeftButton )
        {
            m_set->paintMask( pt.x(), pt.y(), m_maskRadius, !(e->modifiers() & Qt::ShiftModifier) );
            e->accept();
        }
    }
}

//-----------------------------------------------------------------------------
void RenderSetWidget::mouseMoveEvent( QMouseEvent* e )
{       
    QGLWidget::mouseMoveEvent( e );
    if( e->isAccepted() )
        return;

    if( !m_set )
        return;

    // Normalized coordinates in [-1,-1]-[1,1]
    QPointF pt = normalizedCoordinates(e->pos());
    m_cursorPos = pt;
    
    if( m_state == PickedVertexState )
    {   
        // Handle area editing interaction
        pt -= m_delta;
        if( e->buttons() & Qt::LeftButton )
        {
            m_set->setPickedVertexPosition( pt.x(), pt.y() );
            e->accept();
        }
    }
    else if( m_state == PaintMaskState )
    {
        // Handle mask painting
        if( e->buttons() & Qt::LeftButton )
        {
            m_set->paintMask( pt.x(), pt.y(), m_maskRadius, !(e->modifiers() & Qt::ShiftModifier) );
            e->accept();
        }
    }
}

//-----------------------------------------------------------------------------
void RenderSetWidget::mouseReleaseEvent( QMouseEvent* e )
{   
    QGLWidget::mouseMoveEvent( e );
    if( e->isAccepted() )
        return;

    // Handle area editing interaction
    if( m_state == PickedVertexState )
    {
        if( e->button() == Qt::LeftButton )
        {
            m_state = EditVertexState;
            e->accept();
        }
    }
}

//-----------------------------------------------------------------------------
void RenderSetWidget::wheelEvent( QWheelEvent* e )
{
    // from the Qt doc
    QPoint numPixels = e->pixelDelta();
    QPoint numDegrees = e->angleDelta() / 8;

    if( m_set )
    {
        if( m_state == PaintMaskState )
        {
            if(!numPixels.isNull()) 
            {
                m_maskRadius += numPixels.y();
            } 
            else if(!numDegrees.isNull()) 
            {
                QPoint numSteps = numDegrees / 15;
                m_maskRadius += numSteps.y();
            }
            
            if( m_maskRadius < 5 ) m_maskRadius = 5;
            if( m_maskRadius > 300 ) m_maskRadius = 300;
        }
    }

    e->accept();
}

//-----------------------------------------------------------------------------
void RenderSetWidget::showContextMenu( const QPoint& pt )
{
    if( !m_set ) return;

    QMenu menu;
    menu.addAction(toggleFullscreenAction);
    menu.addSeparator();

    if(true)
    {
        QAction* a1 = menu.addAction(tr("Preview yellow outline"));
        QAction* a2 = menu.addAction(tr("Preview white on black"));
        a1->setCheckable(true);
        a2->setCheckable(true);

        connect(a1, &QAction::triggered, this, [=](){ m_set->setAreaMode( RenderSet::AreaOutline ); });
        connect(a2, &QAction::triggered, this, [=](){ m_set->setAreaMode( RenderSet::AreaBlackWhite ); });

        a1->setChecked(m_set->getAreaMode()==RenderSet::AreaOutline);
        a2->setChecked(m_set->getAreaMode()==RenderSet::AreaBlackWhite);

        menu.addSeparator();
    }

    for(auto action : maskActions)
    {
        menu.addAction(action);
    }
    menu.addSeparator();

    for(auto action : renderActions)
    {
        menu.addAction(action);
    }
    menu.addSeparator();

    QAction* rotateScreenAction = menu.addAction( tr("Rotate screen") );
    connect(rotateScreenAction, &QAction::triggered, this, [=]() 
    {
        m_rotScreen = (m_rotScreen+1)%4;
        if( m_rotScreen%2 )
            this->resize( 480, 640 );
        else
            this->resize( 640, 480 );
    });

    menu.exec( mapToGlobal(pt) );
}

