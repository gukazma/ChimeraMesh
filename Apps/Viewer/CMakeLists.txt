set(TARGET_NAME Viewer)

# just for qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        REQUIRED)


find_package(OpenSceneGraph 2.0.0 REQUIRED osgViewer osgDB osgUtil osgQt5 osgGA)
find_package(Boost COMPONENTS filesystem system thread REQUIRED)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR})
set(QRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Resources/resources.qrc)
set(APPRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Resources/appicon.rc)
file(GLOB_RECURSE CPP_HEAD_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE CPP_SOUCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE CU_SOUCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cu)
file(GLOB_RECURSE UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CPP_HEAD_FILES} ${CPP_SOUCE_FILES} ${CU_SOUCE_FILES} ${UI_FILES})

add_executable(${TARGET_NAME} ${CPP_HEAD_FILES} ${CPP_SOUCE_FILES} ${CU_SOUCE_FILES} ${UI_FILES} ${QRC_FILE} ${APPRC_FILE})

# add_compile_definitions("PILOT_ROOT_DIR=${BINARY_ROOT_DIR}")

set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME ${TARGET_NAME})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Apps)

target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

target_link_libraries(${TARGET_NAME} PRIVATE 
Qt5::Core
Qt5::Gui
Qt5::Widgets
Qt5::OpenGL
Boost::system
Boost::filesystem
Boost::dynamic_linking
Boost::thread
${OPENSCENEGRAPH_LIBRARIES}
)

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  ${OPENSCENEGRAPH_INCLUDE_DIRS}
)