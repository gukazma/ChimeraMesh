#pragma once
#include <QObject>
#include <osg/ref_ptr>
#include <vector>
#include <boost/filesystem.hpp>
namespace osg {
class Group;
class Node;
class PositionAttitudeTransform;
}   // namespace osg
namespace osgViewer {
class View;
}
namespace osgGA {
class MultiTouchTrackballManipulator;
}

class Trajectory;

class SceneManager : public QObject
{
    Q_OBJECT
public:
    SceneManager();
    bool openFile(const boost::filesystem::path& path);
    void upodateRoot();
    void initPresetModel();

    osgViewer::View*                             view;
    osg::Group*                                  root;
    osg::ref_ptr<osg::PositionAttitudeTransform> pre_camera;
    std::vector<osg::Node*>                      nodes;
    std::vector<Trajectory*>                     trajectories;
    osgGA::MultiTouchTrackballManipulator*       manipulator;
public slots:
    void slot_openMesh(const boost::filesystem::path& path);
    void slot_scale();
};


extern SceneManager g_sceneManager;