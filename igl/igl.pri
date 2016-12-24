DEFINES += IGL_DEFINED
CONFIG += IGL_DEFINED
MODULES += IGL

#comment next line if libigl is not used in static mode
CONFIG += IGL_STATIC_LIBRARY

!contains(DEFINES, COMMON_DEFINED){
    error(Igl module requires common module!)
}

unix:!macx{
    QMAKE_CXXFLAGS += -std=c++11  -fopenmp
    QMAKE_LFLAGS +=  -fopenmp
    LIBS += -lboost_system -DBOOST_LOG_DYN_LINK -lboost_log -lboost_thread -lpthread
    INCLUDEPATH += $$(LIBIGL)/include/

    #newest versions of eigen are not supported by libigl
    INCLUDEPATH -= /usr/include/eigen3
    INCLUDEPATH += $$(LIBIGL)/external/nanogui/ext/eigen/

    IGL_STATIC_LIBRARY {
        DEFINES += IGL_STATIC_LIBRARY
        LIBS += -L/usr/include/libiglbin -liglcgal -ligl
    }
}



HEADERS += \
    $$PWD/iglinterface.h \
    $$PWD/iglmesh.h \
    $$PWD/utils.h

SOURCES += \
    $$PWD/iglinterface.cpp \
    $$PWD/iglmesh.cpp

#Note: QtCreator always shows this module included, however files included in it are compiled only if Viewer module is included
VIEWER_DEFINED{
    include ($$PWD/gui/iglmeshmanager.pri)
    CGAL_DEFINED {
        include ($$PWD/gui/booleansmanager.pri)
    }
}
