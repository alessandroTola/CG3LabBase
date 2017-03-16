#include "polylinesCheck.h"

PolylinesCheck::PolylinesCheck(){
}

void PolylinesCheck::minMaxPoints (const Mesh &mesh, int selection){
    PointC i;
    Vec3 normalPlane;
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

void PolylinesCheck::setMin(PointC& minP){
    min = minP;
}

void PolylinesCheck::setMax(PointC& maxP){
    max = maxP;
}

PointC PolylinesCheck::getMin(){
    return min;
}

PointC PolylinesCheck::getMax(){
    return max;
}

void PolylinesCheck::setPoly(const Mesh& mesh, Vec3& norm, double d){
    poly= CGALInterface::Slicer::getPolylines2(mesh, norm, d);
}

void PolylinesCheck::convertTo2dPlane (){

}


