#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../common/serialize.h"
#include "../common/arrays.h"
#include <igl/copyleft/marching_cubes.h>
#include <igl/signed_distance.h>
#include <igl/read_triangle_mesh.h>

#include <igl/decimate.h>

#include "iglmesh.h"

#include <Eigen/Core>
#include <iostream>

namespace IGLInterface {
    bool generateGridAndDistanceField(const std::string &s);
    void generateGridAndDistanceField(const SimpleIGLMesh &m, Array3D<Pointd>& grid, Array3D<double> &distanceField);
}


#endif // FUNCTIONS_H
