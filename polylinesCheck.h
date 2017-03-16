#ifndef POLYLINES_H
#define POLYLINES_H

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


using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel             K;
typedef Simple_cartesian<double>                                  KK;
typedef Surface_mesh<K::Point_3>                                  Mesh;
typedef K::Point_3                                                PointC;
typedef K::Point_2                                                PointD;
typedef K::Plane_3                                                PlaneC;
typedef Mesh::Vertex_index                                        vertex_descriptor;
typedef Mesh::Face_index                                          face_descriptor;
typedef Mesh::Vertex_range                                        verticesIter;
typedef Mesh::Property_map<vertex_descriptor, K::Point_3>         MapPoints;
typedef std::vector<std::vector<Pointd>>                          ArrayPoint;
typedef std::vector<PointD>                                             Array2dPoint;
class PolylinesCheck
{
    public:
        PolylinesCheck();

        ArrayPoint poly;

        Array2dPoint poly2d;

        void minMaxPoints (const Mesh& mesh, int selection);

        void setMin(PointC& minP);

        void setMax(PointC& maxP);

        PointC getMin();

        PointC getMax();

        void setPoly(const Mesh& mesh, Vec3& norm, double d);

        void convertTo2dPlane ();

    private:

        PointC min;
        PointC max;
        Vec3 normalplane;
};

#endif // POLYLINES_H
