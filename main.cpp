//Test common.pri:
#include "common/arrays.h"
#include "common/bounding_box.h"
#include "common/comparators.h"
#include "common/drawable_object.h"
#include "common/pickable_object.h"
#include "common/timer.h"

//Test viewer.pri:
#include "GUI/mainwindow.h"
#include "GUI/managers/windowmanager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow gui;  // finestra principale, contiene la canvas di QGLViewer

    // Creo un window manager e lo aggiungo alla mainwindow
    WindowManager wm(&gui);
    int id = gui.addManager(&wm, "Window");

    gui.setCurrentIndexToolBox(id); // il window manager sarà quello visualizzato di default
    gui.updateGlCanvas();
    gui.show();

    return app.exec();
}
