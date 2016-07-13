#ifndef CGALINTERFACE_H
#define CGALINTERFACE_H

#include "../common/common.h"
#include "../common/point.h"
#include "../common/point2d.h"

#ifdef CGAL_DEFINED
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#endif //CGAL_DEFINED

/**
  CGAL Triangulation
  */
#ifdef CGAL_DEFINED
namespace CGALInterface {

    namespace Triangulation{
        struct FaceInfo2
        {
                FaceInfo2(){}
                int nesting_level;
                bool in_domain(){
                    return nesting_level%2 == 1;
                }
        };


        typedef CGAL::Exact_predicates_inexact_constructions_kernel             K;
        typedef CGAL::Triangulation_vertex_base_2<K>                            Vb;
        typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2,K>          Fbb;
        typedef CGAL::Constrained_triangulation_face_base_2<K,Fbb>              Fb;
        typedef CGAL::Triangulation_data_structure_2<Vb,Fb>                     TDS;
        typedef CGAL::Exact_predicates_tag                                      Itag;
        typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag>        CDT;
        typedef CDT::Point                                                      CGALPoint;
        typedef CGAL::Polygon_2<K>                                              Polygon_2;

        typedef CGAL::Epick                                                     E;
        typedef CGAL::Triangulation_ds_face_base_2<TDS>                         TDFB2;
        typedef CGAL::Triangulation_face_base_2<E, TDFB2>                       TFB2;
        typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2, E, TFB2>   TFBI;
        typedef CGAL::Constrained_triangulation_face_base_2<E, TFBI >           Triangle;

        static std::vector< std::vector<Point2D> > dummy_holes2D;
        static std::vector< std::vector<Pointd> > dummy_holes;

        void markDomains(CDT& ct, CDT::Face_handle start, int index, std::list<CDT::Edge>& border);
        void markDomains(CDT& cdt);
        void triangulate(std::vector<std::array<Point2D, 3> >& triangles, const std::vector<Point2D>& polygon, const std::vector<std::vector<Point2D> >& holes = dummy_holes2D);
        void triangulate(std::vector<std::array<Pointd, 3> >& triangles, const Vec3 &normal, const std::vector<Pointd>& polygon, const std::vector<std::vector<Pointd> >& holes = dummy_holes);
    }

    namespace BooleanOperations{

        typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
        typedef Kernel::Point_2                                   Point_2;
        typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
        typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
        typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;

        void getCoordinates(const Point_2&p, double &x, double &y);

        void difference(std::vector< std::vector<Point2D> > &result, const std::vector<Point2D> &polygon1, const std::vector<Point2D> &polygon2);
        void intersection(std::vector<std::vector<Point2D> >& result, const std::vector<Point2D>& polygon1, const std::vector<Point2D>& polygon2);
    }

    class AABBTree;


}
#endif //CGAL_DEFINED

#endif // CGALINTERFACE_H
