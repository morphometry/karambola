// calculate W210,  integral dA  (1/6) (1/R1 + 1/R2)  r
#include "calculate.h"
#include <math.h>
#include <cassert>

CompWiseVectorMinkValResultType calculate_w210(const Triangulation& surface){
    CompWiseVectorMinkValResultType w210;
    //calculate w210
    for (unsigned int i = 0; i<3;i++){
        for (unsigned int j = 0; j<surface.n_triangles();j++){
            for (unsigned int k = 0; k<3;k++){
                double local_value = 0;
                if(surface.ith_neighbour_of_triangle(j,k) != NEIGHBOUR_UNASSIGNED){
                    Vector a = surface.normal_vector_of_triangle(surface.ith_neighbour_of_triangle(j,k));
                    Vector b = surface.normal_vector_of_triangle(j);
                    double c = dot(a,b)/(norm(a)*norm(b));
                    assert (c <= 1.000001);
                    if (c>= 1) c=1.;
                    double alpha = acos(c);

                    Vector convex = surface.com_of_triangle(j) + b - (surface.com_of_triangle(surface.ith_neighbour_of_triangle(j,k)) + a);
                    Vector concave = surface.com_of_triangle(j) - b - (surface.com_of_triangle(surface.ith_neighbour_of_triangle(j,k)) - a);
                    if(norm(convex) < norm(concave)) alpha = -alpha;

                    double e = surface.get_edge_length(j,k);
                    double w120_part_c1 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(j,k))[i];
                    double w120_part_c2 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(j,(k+1)%3))[i];
                    double w120_part = alpha * e *( w120_part_c1 + w120_part_c2);
                    local_value = w120_part/(12.*2.);
                }
                w210[surface.label_of_triangle(j)].result_[i] += local_value;
            }
        }
    }
    return w210;
}

