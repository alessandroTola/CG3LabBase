#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H

#include <viewer/interfaces/drawable_mesh.h>
#include <viewer/mainwindow.h>

#include <eigenmesh/eigenmesh/gui/drawableeigenmesh.h>
#include <eigenmesh/eigenmesh/eigenmesh.h>
#include <Eigen/Dense>

#include <CGAL/Surface_mesh.h>
#include <CGAL/aabbtree.h>
#include <CGAL/cgalinterface.h>
#include <CGAL/Cartesian/Cartesian_base.h>
#include <CGAL/Surface_mesh/IO.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>


#include <QFrame>

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif


class QLineEdit;
using namespace Eigen;
using namespace CGAL;

namespace Ui {
    class DrawManager;
}

typedef Simple_cartesian<double>     K;
typedef Surface_mesh<K::Point_3>  Mesh;
typedef IO output;

class DrawManager : public QFrame
{
        Q_OBJECT

    public:
        explicit DrawManager(QWidget *parent = 0);
        ~DrawManager();

    private slots:

        void on_x_editingFinished();

        void on_y_editingFinished();

        void on_z_editingFinished();

        void on_drawAxis_clicked();

        void on_loadMesh_clicked();

        void on_eigenToCgal_clicked();

        void convertEigenMesh(DrawableEigenMesh *meshEigenOrigin);

        void on_drawCilinder_clicked();

        void on_saveMeshCgal_clicked();

private:

        Ui::DrawManager *ui;
        MainWindow* mainWindow;
        Vec3 vectorUser;
        DrawableEigenMesh meshEigen;
        Mesh mesh;

};

#endif // DRAWMANAGER_H
