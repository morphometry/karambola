#include "calculate.h"
#include <math.h>
#include <cassert>

CompWiseVectorMinkValResultType calculate_w310(const Triangulation& surface){
    CompWiseVectorMinkValResultType w310;
    //calculate w310
    for (unsigned int i = 0; i<3;i++){
        for (unsigned int j = 0; j<surface.n_vertices();j++){
            double sum = surface.sum_of_angles_of_ith_vertex(j);
            for (unsigned int l = 0; l<surface.get_triangles_of_vertex(j).size();l++){
                for(unsigned int k= 0; k<3;k++){
                    if(surface.ith_vertex_of_triangle(surface.get_triangles_of_vertex(j).at(l),k)==j){
                        double angle = surface.get_angle_of_ith_edge(surface.get_triangles_of_vertex(j).at(l),k);
                        double w310_part = ((2*M_PI*(angle/sum)) - angle)*surface.get_pos_of_vertex(j)[i];

                        w310[surface.label_of_triangle(surface.get_triangles_of_vertex(j).at(l))].result_[i] += w310_part/3.;
                    }
                }
            }
        }
    }
    return w310;
}

