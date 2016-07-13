#Add or remove all the modules you need

#Common module: contains classes and common functions used on all the other modules
#Requires: Eigen
include (common/common.pri)

#Viewer module: contains classes for a simple viewer
#Requires: Common module, libQGLViewer, boost
include (GUI/viewer.pri)

#Dcel module: contains a Double Connected-Edge List data structure
#Requires: Common module, boost; Optional: Cgal module, viewer module
include (dcel/dcel.pri)

#Cgal module: contains an interface to some functionalities of CGAL library
#Requires: Common module, libCgal; Optional: Dcel module
include (cgal_interface/cgal.pri)

#Trimesh module:
include (trimesh/trimesh.pri)

SOURCES += \
    main.cpp
