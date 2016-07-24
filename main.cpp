///Test common.pri: uncomment to test common module
#include "common/arrays.h"
#include "common/bounding_box.h"
#include "common/comparators.h"
#include "viewer/interfaces/drawable_object.h"
#include "viewer/interfaces/pickable_object.h"
#include "common/timer.h"

///Test viewer.pri: uncomment to test viewer module
#include "viewer/mainwindow.h"
#include "viewer/managers/windowmanager.h"

///Test dcel.pri: uncomment to test dcel module
//#include "dcel/dcel.h"
///Test dcelviewer.pri: uncomment to test dcel module
//#include "dcel/gui/dcelmanager.h"

//Test trimesh.pri: uncomment to test trimehs module
#include "trimesh/trimesh.h"
///Test trimeshviewer.pri: uncomment to test trimesh module
#include "trimesh/gui/trimeshmanager.h"

int main(int argc, char *argv[]) {

    ///Test viewer.pri: uncomment to test viewer module
    QApplication app(argc, argv);

    MainWindow gui;  // finestra principale, contiene la canvas di QGLViewer


    WindowManager wm(&gui); // Creo un window manager e lo aggiungo alla mainwindow
    int id = gui.addManager(&wm, "Window");

    //Test trimeshmanager.pri
    TrimeshManager tm(&gui);
    id = gui.addManager(&tm, "Trimesh");

    gui.setCurrentIndexToolBox(id); // il window manager sarà quello visualizzato di default
    gui.updateGlCanvas();
    gui.show();

    return app.exec();
}
