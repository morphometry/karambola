#include "calculate.h"
#include "../lib/Vector.h"
#include "../lib/Kmath.h"
#include <cassert>

CompWiseVectorMinkValResultType calculate_w010(const Triangulation& surface){
    CompWiseVectorMinkValResultType w010;
    //calculate w010
    for (unsigned int i = 0; i<3;i++){
        for (unsigned int j = 0; j<surface.n_triangles();j++){
            Vector c1 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(j,0));
            Vector c2 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(j,1));
            Vector c3 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(j,2));
            Vector v = c2 - c1;
            Vector w = c3 - c1;
            double part1 = 2*v[i]*v[(i+1)%3] + v[(i+1)%3]*w[i] + 4*c1[(i+1)%3]*(v[i]+w[i]) + v[i]*w[(i+1)%3] + 2*w[i]*w[(i+1)%3] + 4*c1[i]*(3*c1[(i+1)%3]+v[(i+1)%3]+w[(i+1)%3]);
            double vf = cross_product (v,w)[(i+1)%3];
            w010[surface.label_of_triangle(j)].result_[i] += vf*part1/ 24.;
        }
    }
    return w010;
}

