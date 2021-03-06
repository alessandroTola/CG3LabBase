#ifndef POLYLINES_H
#define POLYLINES_H

#include <eigenmesh/eigenmesh/gui/drawableeigenmesh.h>
#include <eigenmesh/eigenmesh/gui/eigenmeshmanager.h>
#include <eigenmesh/eigenmesh/eigenmesh.h>
#include <eigenmesh/eigenmesh/gui/pickable_eigenmesh.h>
#include <Eigen/Dense>

#include <CGAL/Surface_mesh.h>
#include <cgal/aabbtree.h>
#include <CGAL/Cartesian/Cartesian_base.h>
#include <CGAL/Surface_mesh/IO.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Filtered_kernel.h>
#include <cgal/cgalslicer.h>
#include <common/utils.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>
#include <QFrame>
#include <gurobi_c++.h>

#include <algorithm>

using namespace CGAL;
using namespace std;

typedef Exact_predicates_inexact_constructions_kernel             K;
typedef Simple_cartesian<double>                                  KK;
typedef Surface_mesh<K::Point_3>                                  Mesh;
typedef K::Point_3                                                Point3;
typedef K::Point_2                                                Point2;
typedef K::Plane_3                                                PlaneC;
typedef Mesh::Vertex_index                                        vertex_descriptor;
typedef Mesh::Face_index                                          face_descriptor;
typedef Mesh::Vertex_range                                        verticesIter;
typedef Mesh::Property_map<vertex_descriptor, Point3>             MapPoints;
typedef std::vector<std::vector<Pointd>>                          ArrayPoint;
typedef std::vector<Point2>                                       Array2dPoint;
typedef Eigen::Matrix3d                                           Matrix;
typedef std::vector<int>                                          VectI;
typedef std::vector<VectI>                                        MatrixI;

class PolylinesCheck
{
    public:
        PolylinesCheck();

        //Pointd  maxP;

        void    minMaxPoints            (const Mesh& mesh, int selection);

        void    setMin                  (Pointd& minP1);

        void    setMax                  (Pointd& maxP1);

        Pointd  getMin                  ();

        Pointd  getMax                  ();

        void    setPoly                 (const Mesh& mesh, Vec3& norm);

        void    convertTo2dPlane        ();

        bool    checkPolyline           ();

        void    setNormal               (Vec3& normal);

        void    setD                    ();

        int     intersect3D_RayTriangle (Pointd p0, Pointd p1, Pointd v0, Pointd v1, Pointd v2);

        void    checkIntersect          (DrawableEigenMesh* meshEigenOrigin, Pointd p0, Pointd p1, int selection);

        void    rotatePoint             (Eigen::Matrix3d rotation, Pointd p);

        void    check                   (DrawableEigenMesh *meshEigenOrigin, int color, int indexPlane);

        int     serchMinY               (std::vector<int> lista, DrawableEigenMesh *meshEigenOrigin);

        int     serchMaxY               (std::vector<int> lista, DrawableEigenMesh *meshEigenOrigin);

        void    setCheckerDimension     (int nplane, int dimension);

        void    resetChecker            ();

        MatrixI   getChecker            ();

        void searchNoVisibleFace        ();

        VectI getNotVisibleFace         ();

        void minimizeProblem            ();

        void updateChecker              (bool b);

        void resetMatrixCheck();

        void serchUniqueTriangoForOrientation();
        MatrixI getUniqueTriangle() const;
        void setUniqueTriangle(const MatrixI &value);

        VectI getOrientationSelected() const;

        void addFaceExlude(unsigned int i);

private:

        PickableEigenmesh   meshPoly;
        Array2dPoint        poly2d;
        ArrayPoint          poly;
        VectI               notVisibleFace;
        VectI               orientationSelected;
        Point3              min;
        Point3              max;
        Pointd              minP;
        Pointd              maxP;
        Pointd              I;
        MatrixI             checker;
        MatrixI             uniqueTriangle;
        Vec3                normalplane;
        double              d;
};

#endif // POLYLINES_H
