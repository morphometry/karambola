#include "calculate.h"
#include "../lib/Kmath.h"
#include <cassert>

CompWiseMatrixMinkValResultType calculate_w020( const Triangulation& surface,
                                                const CompWiseScalarMinkValResultType& w000,
                                                const CompWiseVectorMinkValResultType& w010){
    CompWiseMatrixMinkValResultType w020;
    //calculate w020
    for (unsigned int k = 0; k < surface.n_triangles(); k++){

        //(0,0,0) for default value
        Vector ref_vec(0,0,0);
        if (w000.size() != 0)
            ref_vec = get_ref_vec(surface.label_of_triangle(k),w000,w010);

        Vector c1 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(k,0)) - ref_vec;
        Vector c2 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(k,1)) - ref_vec;
        Vector c3 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(k,2)) - ref_vec;
        Vector n = surface.normal_vector_of_triangle(k);
        double area = surface.area_of_triangle(k);

        double Ixx = (3*pow(c2[0],2)*c2[2] + 2*c2[0]*c2[2]*c3[0] + c2[2]*pow(c3[0],2) + c1[2]*(pow(c2[0],2) + c2[0]*c3[0] + pow(c3[0],2)) + pow(c2[0],2)*c3[2] + 2*c2[0]*c3[0]*c3[2] + 3*pow(c3[0],2)*c3[2] + pow(c1[0],2)*(3*c1[2] + c2[2] + c3[2]) + c1[0]*(2*c1[2]*(c2[0] + c3[0]) + c2[0]*(2*c2[2] + c3[2]) + c3[0]*(c2[2] + 2*c3[2])))/60.;
        w020[surface.label_of_triangle(k)].result_(0,0) += Ixx*2*area*n[2];

        double Iyy = (3*pow(c2[1],2)*c2[2] + 2*c2[1]*c2[2]*c3[1] + c2[2]*pow(c3[1],2) + c1[2]*(pow(c2[1],2) + c2[1]*c3[1] + pow(c3[1],2)) + pow(c2[1],2)*c3[2] + 2*c2[1]*c3[1]*c3[2] + 3*pow(c3[1],2)*c3[2] + pow(c1[1],2)*(3*c1[2] + c2[2] + c3[2]) + c1[1]*(2*c1[2]*(c2[1] + c3[1]) + c2[1]*(2*c2[2] + c3[2]) + c3[1]*(c2[2] + 2*c3[2])))/60.;
        w020[surface.label_of_triangle(k)].result_(1,1) += Iyy*2*area*n[2];

        double Izz = (3*c2[1]*pow(c2[2],2) + pow(c2[2],2)*c3[1] + pow(c1[2],2)*(c2[1] + c3[1]) + 2*c2[1]*c2[2]*c3[2] + 2*c2[2]*c3[1]*c3[2] + c2[1]*pow(c3[2],2) + 3*c3[1]*pow(c3[2],2) + c1[1]*(3*pow(c1[2],2) + pow(c2[2],2) + c2[2]*c3[2] + pow(c3[2],2) + 2*c1[2]*(c2[2] + c3[2])) + c1[2]*(c2[1]*(2*c2[2] + c3[2]) + c3[1]*(c2[2] + 2*c3[2])))/60.;
        w020[surface.label_of_triangle(k)].result_(2,2) += Izz*2*area*n[1];

        double Ixy = (2*c1[2]*c2[0]*c2[1] + 6*c2[0]*c2[1]*c2[2] + c1[2]*c2[1]*c3[0] + 2*c2[1]*c2[2]*c3[0] + c1[2]*c2[0]*c3[1] + 2*c2[0]*c2[2]*c3[1] + 2*c1[2]*c3[0]*c3[1] + 2*c2[2]*c3[0]*c3[1] + 2*c2[0]*c2[1]*c3[2] + 2*c2[1]*c3[0]*c3[2] + 2*c2[0]*c3[1]*c3[2] + 6*c3[0]*c3[1]*c3[2] + c1[0]*(2*c2[1]*c2[2] + c2[2]*c3[1] + 2*c1[2]*(c2[1] + c3[1]) + c2[1]*c3[2] + 2*c3[1]*c3[2] + 2*c1[1]*(3*c1[2] + c2[2] + c3[2])) + c1[1]*(2*c1[2]*(c2[0] + c3[0]) + c2[0]*(2*c2[2] + c3[2]) + c3[0]*(c2[2] + 2*c3[2])))/120.;

        w020[surface.label_of_triangle(k)].result_(0,1) += Ixy*2*area*n[2];
        w020[surface.label_of_triangle(k)].result_(0,2) += Ixy*2*area*n[1];
        w020[surface.label_of_triangle(k)].result_(1,2) += Ixy*2*area*n[0];
    }

    return w020;
}

