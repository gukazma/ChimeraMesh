#include <QTimer>
#include <QWidget>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/CompositeViewer>
class OSGWidget : public QWidget, public osgViewer::CompositeViewer
{
public:
    OSGWidget(QWidget* parent = 0, Qt::WindowFlags f = 0,
              osgViewer::ViewerBase::ThreadingModel threadingModel =
                  osgViewer::CompositeViewer::SingleThreaded);

    QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw, osg::ref_ptr<osg::Node> scene);

    osgQt::GraphicsWindowQt* createGraphicsWindow(int x, int y, int w, int h,
                                                  const std::string& name             = "",
                                                  bool               windowDecoration = false);

    virtual void paintEvent(QPaintEvent* /*event*/) { frame(); }

protected:
    QTimer _timer;
};
