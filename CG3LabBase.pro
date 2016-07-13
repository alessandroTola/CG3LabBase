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

#Common module: contains classes and common functions used on all the other modules
#Requires: Eigen
include (common/common.pri)

#Viewer module: contains classes for a simple viewer
#Requires: Common module, libQGLViewer, boost
include (viewer/viewer.pri)

#Dcel module: contains a Double Connected-Edge List data structure
#Requires: Common module, boost; Optional: Cgal module, viewer module
#include (dcel/dcel.pri)

#Cgal module: contains an interface to some functionalities of CGAL library
#Requires: Common module, libCgal; Optional: Dcel module
#include (cgal/cgal.pri)

#Trimesh module: contains a Trimesh data structure
include (trimesh/trimesh.pri)

#Igl module: coontaint an intergace to some functionalities of libIGL
#Requires: Common module, libIGL (an environment variable named LIBIGL containing the root directory of the library must be setted)
#include (igl/igl.pri)

SOURCES += \
    main.cpp
