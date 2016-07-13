CONFIG(debug, debug|release){
    DEFINES += DEBUG
}
CONFIG(release, debug|release){
    DEFINES -= DEBUG
    unix:!macx{ #just uncomment next lines if you want to ignore asserts and got a more optimized binary
        #QMAKE_CXXFLAGS_RELEASE -= -g
        #QMAKE_CXXFLAGS += -O3 -DNDEBUG
    }
}

#Add or remove all the modules you need
#Before pushing the project with your new module, please double check that everything works keeping uncommentend
#only the modules that are required by your module. Also please write here required and optional modules for your module

#Common module: contains classes and common functions used on all the other modules
#Requires: Eigen
include (common/common.pri)

#Viewer module: contains classes for a simple viewer
#Requires: Common module, libQGLViewer, boost
include (viewer/viewer.pri)

#Dcel module: contains a Double Connected-Edge List data structure
#Requires: Common module, boost;
#Optional: Cgal module, viewer module
#WARNING: to use the dcel module without the viewer, double check the dcel.pri file (qmake bug, sorry!)
#include (dcel/dcel.pri)

#Cgal module: contains an interface to some functionalities of CGAL library
#Requires: Common module, libCgal; Optional: Dcel module
#include (cgal/cgal.pri)

#Trimesh module: contains a Trimesh data structure
#Requires: Common module
#Oprional: Viewer module
#WARNING: to use the trimesh module without the viewer, double check the trimesh.pri file (qmake bug, sorry!)
include (trimesh/trimesh.pri)

#Igl module: coontaint an intergace to some functionalities of libIGL
#Requires: Common module, libIGL (an environment variable named LIBIGL containing the root directory of the library must be setted)
#Oprional: Viewer module
#WARNING: to use the igl module without the viewer, double check the igl.pri file (qmake bug, sorry!)
#include (igl/igl.pri)

SOURCES += \
    main.cpp
