#include "calculate.h"
#include <math.h>

CompWiseMatrixMinkValResultType calculate_w320( const Triangulation& surface,
                                                const CompWiseScalarMinkValResultType& w300,
                                                const CompWiseVectorMinkValResultType& w310){

    CompWiseMatrixMinkValResultType w320;

    //calculate w320
    for (unsigned int m = 0; m<surface.n_vertices();m++){
        double sum = surface.sum_of_angles_of_ith_vertex(m);
        for (unsigned int l = 0; l<surface.get_triangles_of_vertex(m).size();l++){
            for(unsigned int k= 0; k<3;k++){
                if(surface.ith_vertex_of_triangle(surface.get_triangles_of_vertex(m).at(l),k)==m){
                    double angle = surface.get_angle_of_ith_edge(surface.get_triangles_of_vertex(m).at(l),k);
                    double angle_part = ((2*M_PI*(angle/sum)) - angle);

                    Vector ref_vec(0,0,0);
                    if (w300.size() != 0)
                        ref_vec = get_ref_vec(surface.label_of_triangle(surface.get_triangles_of_vertex(m).at(l)),w300,w310);

                    Vector c = surface.get_pos_of_vertex(m) - ref_vec;
                    for (unsigned int i = 0; i<3;i++){
                        for (unsigned int j = 0; j<=i;j++){
                            double w320_part = angle_part * c[i] * c[j];
                            w320[surface.label_of_triangle(surface.get_triangles_of_vertex(m).at(l))].result_(i,j) += w320_part/3.;
                        }
                    }
                }
            }
        }
    }

    return w320;
}

