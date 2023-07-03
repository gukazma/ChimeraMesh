#include "SceneManager.h"
#include "Widgets/ProgressBar.h"
#include <Widgets/GlobalSignalEmiiter.h>
#include <osg/Group>
#include <osg/LineWidth>
#include <osg/PolygonMode>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <thread>
SceneManager g_sceneManager;

SceneManager::SceneManager()
    : view(new osgViewer::View)
    , root(new osg::Group)
    , manipulator(new osgGA::MultiTouchTrackballManipulator)
{
    view->setSceneData(root);
    view->addEventHandler(new osgViewer::StatsHandler);
    view->setCameraManipulator(manipulator, false);

    initPresetModel();
}

bool SceneManager::openFile(const boost::filesystem::path& path)
{
    auto node = osgDB::readNodeFile(path.generic_string());
    if (node == nullptr) {
        return false;
    }
    else {
        nodes.push_back(node);
        upodateRoot();
    }
    return true;
}


void SceneManager::upodateRoot()
{
    // root->removeChildren(0, root->getNumChildren());
    root = new osg::Group;
    for (auto&& node : nodes) {
        root->addChild(node);
    }
    view->setSceneData(root);
    view->setCameraManipulator(manipulator);
}

void SceneManager::initPresetModel()
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-0.596314, -0.596314, 1.000000));
    vertices->push_back(osg::Vec3(-0.596314, 0.596314, 1.000000));
    vertices->push_back(osg::Vec3(-1.000000, -1.000000, -1.000000));
    vertices->push_back(osg::Vec3(-1.000000, 1.000000, -1.000000));
    vertices->push_back(osg::Vec3(0.596314, -0.596314, 1.000000));
    vertices->push_back(osg::Vec3(0.596314, 0.596314, 1.000000));
    vertices->push_back(osg::Vec3(1.000000, -1.000000, -1.000000));
    vertices->push_back(osg::Vec3(1.000000, 1.000000, -1.000000));

    osg::ref_ptr<osg::DrawElementsUInt> indices =
        new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 20);
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(3);
    indices->push_back(2);
    indices->push_back(6);
    indices->push_back(7);
    indices->push_back(5);
    indices->push_back(4);
    indices->push_back(4);
    indices->push_back(5);
    indices->push_back(1);
    indices->push_back(0);
    indices->push_back(2);
    indices->push_back(6);
    indices->push_back(4);
    indices->push_back(0);
    indices->push_back(7);
    indices->push_back(3);
    indices->push_back(1);
    indices->push_back(5);

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
    geometry->setVertexArray(vertices);
    geometry->addPrimitiveSet(indices);

    // 创建位置姿态变换节点
    pre_camera = new osg::PositionAttitudeTransform;
    pre_camera->setPosition(osg::Vec3d(0.0, 0.0, 0.0));
    pre_camera->setAttitude(osg::Quat(osg::DegreesToRadians(45.0), osg::Vec3d(1.0, 0.0, 0.0)));

    // 将几何体添加到位置姿态变换节点
    pre_camera->addChild(geometry);

    // 获取或创建osg::StateSet并将其设置为禁用光照
    osg::ref_ptr<osg::StateSet> stateSet = geometry->getOrCreateStateSet();
    stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    // 使用osg::LineWidth和osg::PolygonMode来设置线宽和多边形绘制模式，以便将模型渲染为线框模式
    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth(2.0);
    stateSet->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);

    osg::ref_ptr<osg::PolygonMode> polygonMode =
        new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
    stateSet->setAttributeAndModes(polygonMode, osg::StateAttribute::ON);
}

void SceneManager::slot_scale()
{
    view->setCameraManipulator(manipulator);
}

void SceneManager::slot_openMesh(const boost::filesystem::path& path)
{
    ProgressBar::ThreadFunction func = ([&]() {
        std::cout << "Opening mesh..." << std::endl;
        openFile(path);
        std::cout << "Open mesh successfully..." << std::endl;
        g_done                = true;
        g_progress_percentage = 100;
    });
    ProgressBar                 progressbar(func, "Opening mesh,please wait.");
    progressbar.exec();
}