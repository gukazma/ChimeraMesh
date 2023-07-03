#include "OSGWidget.h"
#include <QApplication>
#include <QGridLayout>
#include <QTimer>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/MultiTouchTrackballManipulator>

#include <osgDB/ReadFile>

#include <osgQt/GraphicsWindowQt>

#include "Scene/SceneManager.h"
#include <iostream>
OSGWidget::OSGWidget(QWidget* parent, Qt::WindowFlags f,
                     osgViewer::ViewerBase::ThreadingModel threadingModel)
    : QWidget(parent, f)
{
    setThreadingModel(threadingModel);

    // disable the default setting of viewer.done() by pressing Escape.
    setKeyEventSetsDone(0);

    QWidget*     widget1 = addViewWidget(createGraphicsWindow(0, 0, 100, 100), g_sceneManager.root);
    QGridLayout* grid    = new QGridLayout;
    grid->addWidget(widget1, 0, 0);
    setLayout(grid);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(10);
}

QWidget* OSGWidget::addViewWidget(osgQt::GraphicsWindowQt* gw, osg::ref_ptr<osg::Node> scene)
{
    osgViewer::View* view = g_sceneManager.view;
    addView(view);
    osg::Camera* camera = view->getCamera();
    camera->setGraphicsContext(gw);

    const osg::GraphicsContext::Traits* traits = gw->getTraits();

    camera->setClearColor(
        osg::Vec4(0.9529411764705882, 0.9529411764705882, 0.9529411764705882, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective(
        30.0f,
        static_cast<double>(traits->width) / static_cast<double>(traits->height),
        1.0f,
        10000.0f);

    gw->setTouchEventsEnabled(true);
    return gw->getGLWidget();
}

osgQt::GraphicsWindowQt* OSGWidget::createGraphicsWindow(int x, int y, int w, int h,
                                                         const std::string& name,
                                                         bool               windowDecoration)
{
    osg::DisplaySettings*                      ds     = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName                                = name;
    traits->windowDecoration                          = windowDecoration;
    traits->x                                         = x;
    traits->y                                         = y;
    traits->width                                     = w;
    traits->height                                    = h;
    traits->doubleBuffer                              = true;
    traits->alpha                                     = ds->getMinimumNumAlphaBits();
    traits->stencil                                   = ds->getMinimumNumStencilBits();
    traits->sampleBuffers                             = ds->getMultiSamples();
    traits->samples                                   = ds->getNumMultiSamples();

    return new osgQt::GraphicsWindowQt(traits.get());
}