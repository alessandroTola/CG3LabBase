#include "polylinesCheck.h"

#define SMALL_NUM   0.0000000001 // anything that avoids division overflow

PolylinesCheck::PolylinesCheck(){
}

void PolylinesCheck::minMaxPoints (const Mesh &mesh, int selection){
    Point3 i;
    min = mesh.point((vertex_descriptor) 0);
    max = mesh.point((vertex_descriptor) 0);
    MapPoints map = mesh.points();

    switch(selection){

        case(0):
            for(vertex_descriptor vd : mesh.vertices()){
                i=map[vd];
                if(i.x() < min.x()){
                    min = i;
                }

                if(i.x() > max.x()){
                    max = i;
                }
            }
            break;
        case(1):
            for(vertex_descriptor vd : mesh.vertices()){
                i=map[vd];
                if(i.y() < min.y()){
                    min = i;
                }

                if(i.y() > max.y()){
                    max = i;
                }
            }
            break;
        case(2):
            for(vertex_descriptor vd : mesh.vertices()){
                i=map[vd];
                if(i.z() < min.z()){
                    min = i;
                }

                if(i.z() > max.z()){
                    max = i;
                }
            }
            break;
    }
}

void PolylinesCheck::setMin(Pointd& minP1){
    minP = minP1;
}

void PolylinesCheck::setMax(Pointd& maxP1){
    maxP = maxP1;
}

Pointd PolylinesCheck::getMin(){
    return minP;
}

Pointd PolylinesCheck::getMax(){
    return maxP;
}

void PolylinesCheck::setPoly(const Mesh& mesh, Vec3& norm){
    poly = CGALInterface::Slicer::getPolylines2(mesh, norm, d);
}

void PolylinesCheck::convertTo2dPlane (){

    //Rotation of the coordinates
    Vec3 zAxis(0,0,1);
    Vec3 axis = -(normalplane.cross(zAxis));
    axis.normalize();
    double dot = normalplane.dot(zAxis);
    double angle = acos(dot);

    double r[3][3] = {{0}};
    if (normalplane != zAxis){
        if (normalplane == -zAxis){
            axis = Vec3(1,0,0);
        }
        Common::getRotationMatrix(axis, angle, r);
    }
    else {
        r[0][0] = r[1][1] = r[2][2] = 1;
    }

    //rotate points and make 2D polygon
    for (unsigned int i = 0; i < poly[0].size(); ++i){
        Pointd a = poly[0][i];
        Pointd p1(a.x() * r[0][0] + a.y() * r[1][0] +a.z() * r[2][0],
                  a.x() * r[0][1] + a.y() * r[1][1] +a.z() * r[2][1],
                  a.x() * r[0][2] + a.y() * r[1][2] +a.z() * r[2][2]);
        Point2 p(p1.x(), p1.y());
        //polygon2D.push_back(p);
        poly2d.push_back(p);
    }
}

bool PolylinesCheck::checkPolyline (){
    convertTo2dPlane();
    return true;
}

void PolylinesCheck::setNormal(Vec3& normal){
    normalplane = normal;
}

void PolylinesCheck::setD(){
    d = (-max.x()*normalplane.x() -max.y()*normalplane.y()-max.z()*normalplane.z());
}

void PolylinesCheck::checkIntersect(DrawableEigenMesh* meshEigenOrigin, Pointd p0, Pointd p1, int selection){
    int nFaces = meshEigenOrigin->getNumberFaces();
    int inter=0;
    //bool inter = false;
    bool flag = false;
    for(int i=0; i<nFaces;i++){
        Pointi f = meshEigenOrigin->getFace(i);
        inter = intersect3D_RayTriangle(p0, p1, meshEigenOrigin->getVertex(f.x()),
                                                meshEigenOrigin->getVertex(f.y()),
                                                meshEigenOrigin->getVertex(f.z()));
        if(inter == 1){
            if(flag){
                switch (selection) {
                case 0:
                    if(minP.x() > I.x()/*meshEigenOrigin->getVertex(f.x()).x()*/){
                        minP = I/*meshEigenOrigin->getVertex(f.x())*/;
                    }
                    if(maxP.x() < I.x()/*meshEigenOrigin->getVertex(f.x()).x()*/){
                        maxP = I/*meshEigenOrigin->getVertex(f.x())*/;
                    }
                    break;
                case 1:
                    if(minP.y() > I.y()/*meshEigenOrigin->getVertex(f.y()).y()*/){
                        minP = I/*meshEigenOrigin->getVertex(f.y())*/;
                    }
                    if(maxP.y() < I.y()/*meshEigenOrigin->getVertex(f.y()).y()*/){
                        maxP = I/*meshEigenOrigin->getVertex(f.y())*/;
                    }
                    break;
                case 2:
                    if(minP.z() > I.z()/*meshEigenOrigin->getVertex(f.z()).z()*/){
                        minP = I/*meshEigenOrigin->getVertex(f.z())*/;
                    }
                    if(maxP.z() < I.z()/*meshEigenOrigin->getVertex(f.z()).z()*/){
                        maxP = I/*meshEigenOrigin->getVertex(f.z())*/;
                    }
                    break;
                default:
                    break;
                }
            } else {
                minP = I;//meshEigenOrigin->getVertex(f.x());
                maxP = I;//meshEigenOrigin->getVertex(f.x());
                flag = true;
            }
        }
    }
}

int PolylinesCheck::intersect3D_RayTriangle( Pointd p0, Pointd p1, Pointd v0, Pointd v1, Pointd v2){

    // Copyright 2001 softSurfer, 2012 Dan Sunday
    // This code may be freely used and modified for any purpose
    // providing that this copyright notice is included with it.
    // SoftSurfer makes no warranty for this code, and cannot be held
    // liable for any real or imagined damage resulting from its use.
    // Users of this code must verify correctness for their application.
    //
    //intersect3D_RayTriangle(): find the 3D intersection of a ray with a triangle
    //    Input:  a ray R, and a triangle T
    //    Output: *I = intersection point (when it exists)
    //    Return: -1 = triangle is degenerate (a segment or point)
    //             0 =  disjoint (no intersect)
    //             1 =  intersect in unique point I1
    //             2 =  are in the same plane

    Vec3    e1, e2, n;                      // triangle vectors
    Vec3    dir, w0, w;                     // ray vectors
    double     r, a, b;                     // params to calc ray-plane intersect
    //Pointd I;
    // get triangle edge vectors and plane normal
    e1 = v1 - v0;
    e2 = v2 - v0;
    n = e1.cross(e2);                       // cross product
    if (n == (Vec3)0)                       // triangle is degenerate
        return -1;                          // do not deal with this case

    dir = p1 - p0;                          // ray direction vector
    w0 = p0 - v0;
    a = -n.dot(w0);
    b = n.dot(dir);
    if (fabs(b) < SMALL_NUM) {              // ray is  parallel to triangle plane
        if (a == 0)                         // ray lies in triangle plane
            return 2;
        else return 0;                      // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                            // ray goes away from triangle
        return 0;                           // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    I = p0 + Pointd(r*dir.x(), r*dir.y(), r*dir.z());                   // intersect point of ray and plane

    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = e1.dot(e1);
    uv = e1.dot(e2);
    vv = e2.dot(e2);
    w = I - v0;
    wu = w.dot(e1);
    wv = w.dot(e2);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)                 // I is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)           // I is outside T
        return 0;

    return 1;                               // I is in T



}

void PolylinesCheck::check(DrawableEigenMesh *meshEigenOrigin, double color){
    meshPoly = *meshEigenOrigin;

    CGALInterface::AABBTree eigenTree(*meshEigenOrigin);
    std::vector<int> blackList;
    Vec3 e1, e2, e3, n;
    Vec3 a(0,1,0);
    Pointi f;
    int face;
    double max = meshEigenOrigin->getBoundingBox().maxY() + 100 ;
    double min = meshEigenOrigin->getBoundingBox().minY() - 100 ;
    QColor c;

    for(unsigned int i = 0; i < meshEigenOrigin->getNumberFaces(); i++){
        if(checker[i] != 1){
            f = meshEigenOrigin->getFace(i);
            e1 = meshEigenOrigin->getVertex(f.x());
            e2 = meshEigenOrigin->getVertex(f.y());
            e3 = meshEigenOrigin->getVertex(f.z());
            n = meshEigenOrigin->getFaceNormal(i);
            Pointd bar((e1+e2+e3)/3);
            if(bar.y() > 0){
                eigenTree.getIntersectEigenFaces(Pointd(bar.x(), max, bar.z()), Pointd(bar.x(), 0, bar.z()), blackList);
                if(blackList.size() > 0){
                    face = serchMaxY(blackList,meshEigenOrigin);
                    checker[face] = 1;
                    c.setHsv(color, 255,255);
                    meshEigenOrigin->setFaceColor(c.red(), c.green(),c.blue(),face);
                    blackList.clear();
                    continue;
                } else {
                    if(n.dot(a) <= 0){
                        c.setHsv(color, 255,255);
                        meshEigenOrigin->setFaceColor(c.red(), c.green(),c.blue(),i);
                        checker[i] = 1;
                    } else {
                        if(n.dot(a) >= 0){
                            c.setHsv(120+color, 255,255);
                            meshEigenOrigin->setFaceColor(c.red(), c.green(),c.blue(),i);
                            checker[i] = 1;
                        }
                    }
                }qDebug()<< bar.y() << "dio cane2";
            }
            if(bar.y() < 0){
                eigenTree.getIntersectEigenFaces(Pointd(bar.x(), 0, bar.z()), Pointd(bar.x(), min, bar.z()), blackList);
                if(blackList.size() > 0){
                    face = serchMinY(blackList,meshEigenOrigin);
                    checker[face] = 1;
                    c.setHsv(120+color, 255,255);
                    meshEigenOrigin->setFaceColor(c.red(), c.green(),c.blue(),face);
                    blackList.clear();
                    continue;
                } else {
                    if(n.dot(a) <= 0){
                        c.setHsv(color, 255,255);
                        meshEigenOrigin->setFaceColor(c.red(), c.green(),c.blue(),i);
                        checker[i] = 1;
                    } else {
                        if(n.dot(a) >= 0){
                            c.setHsv(120+color, 255,255);
                            meshEigenOrigin->setFaceColor(c.red(), c.green(),c.blue(),i);
                            checker[i] = 1;
                        }
                    }
                }qDebug()<< bar.y() << "dio cane1";
            }
            if(bar.y()==0)qDebug()<< bar.y() << "dio cane3";
            blackList.clear();
        }
    }
}

void PolylinesCheck::rotatePoint(Eigen::Matrix3d rotation, Pointd p){
    minP.rotate(rotation, p);
    maxP.rotate(rotation, p);
}

int PolylinesCheck::serchMaxY (std::vector<int> lista, DrawableEigenMesh *meshEigenOrigin){
    int max = lista.front();
        for(int i : lista){
            if(meshEigenOrigin->getVertex(meshEigenOrigin->getFace(i).y()).y() > meshEigenOrigin->getVertex(meshEigenOrigin->getFace(max).y()).y()){
                max = i;
            }
        }
    return max;
}

int PolylinesCheck::serchMinY (std::vector<int> lista, DrawableEigenMesh *meshEigenOrigin){
        int min = lista.front();
        for(int i : lista){
            if(meshEigenOrigin->getVertex(meshEigenOrigin->getFace(i).y()).y() < meshEigenOrigin->getVertex(meshEigenOrigin->getFace(min).y()).y()){
                min = i;
            }
        }
    return min;
}

void PolylinesCheck::setCheckerDimension (int dimension){
    checker.resize(dimension);
}

std::vector<int>    PolylinesCheck::getChecker  (){
    return checker;
}




