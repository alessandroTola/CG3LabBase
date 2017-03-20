#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H

#include <viewer/interfaces/drawable_object.h>
#include <viewer/interfaces/drawable_mesh.h>
#include <viewer/mainwindow.h>

#include <eigenmesh/eigenmesh/gui/drawableeigenmesh.h>
#include <eigenmesh/eigenmesh/gui/eigenmeshmanager.h>
#include <eigenmesh/eigenmesh/eigenmesh.h>
#include <Eigen/Dense>

#include <CGAL/Surface_mesh.h>
#include <CGAL/aabbtree.h>
#include <CGAL/cgalinterface.h>
#include <CGAL/Cartesian/Cartesian_base.h>
#include <CGAL/Surface_mesh/IO.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Filtered_kernel.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>
#include <QFrame>

#include <polylinesCheck.h>

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
typedef Exact_predicates_inexact_constructions_kernel             K;
typedef Simple_cartesian<double>                                 KK;
typedef Surface_mesh<K::Point_3>                               Mesh;
typedef K::Point_3                                           PointC;
typedef K::Plane_3                                           PlaneC;
typedef Mesh::Vertex_index                        vertex_descriptor;
typedef Mesh::Face_index                            face_descriptor;
typedef Mesh::Vertex_range                             verticesIter;
typedef Mesh::Property_map<vertex_descriptor, K::Point_3> MapPoints;
typedef IO output;

class DrawManager : public QFrame
{
        Q_OBJECT

    public:
        explicit DrawManager(QWidget *parent = 0);
        ~DrawManager();

    private slots:

        void on_x_editingFinished();

        void on_y_editingFinished               ();

        void on_z_editingFinished               ();

        void on_drawAxis_clicked                ();

        void on_loadMesh_clicked                ();

        void on_eigenToCgal_clicked             ();

        void convertEigenMesh                   (DrawableEigenMesh *meshEigenOrigin);

        void on_clearAxis_clicked               ();

        void on_saveMeshCgal_clicked            ();

        void on_nPlane_editingFinished          ();

        void setButtonMeshLoaded                (bool b);

        void on_writeCoordinate_stateChanged    (int arg1);

        void on_showAxis_stateChanged           (int arg1);

        void on_xAxis_toggled                   (bool checked);

        void on_yAxis_toggled                   (bool checked);

        void on_zAxis_toggled                   (bool checked);

        void on_rotationAxis_clicked            ();

        void on_clearMesh_clicked               ();

        void on_xCoord_editingFinished();

        void on_yCoord_editingFinished();

        void on_serchPoint_clicked();

        void on_sphere_editingFinished();

        void on_cylinder_editingFinished();

        void on_drawPoint_clicked();

        void on_translate_clicked();

        void on_check_clicked();

private:

        Ui::DrawManager *ui;
        MainWindow* mainWindow;
        Pointd vectorUser;
        Pointd point;
        Vec2 coordPlane;
        int nPlaneUser;
        int selection;
        DrawableEigenMesh* meshEigen;
        Mesh mesh;
        PolylinesCheck polyline;
        double cylinder, sphere;

};

#endif // DRAWMANAGER_H
