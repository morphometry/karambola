#include "calculate.h"
#include <math.h>
#include <cassert>

CompWiseMatrixMinkValResultType calculate_w120( const Triangulation& surface,
                                                const CompWiseScalarMinkValResultType& w100,
                                                const CompWiseVectorMinkValResultType& w110){
    CompWiseMatrixMinkValResultType w120;

    //calculate w120
    for (unsigned int k = 0; k < surface.n_triangles(); k++){
        double area = surface.area_of_triangle(k);

        Vector ref_vec(0,0,0);
        if (w100.size() != 0)
            ref_vec = get_ref_vec(surface.label_of_triangle(k),w100,w110);

        Vector c1 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(k,0)) - ref_vec;
        Vector c2 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(k,1)) - ref_vec;
        Vector c3 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(k,2)) - ref_vec;
        for (unsigned int i = 0; i<3;i++){
            for (unsigned int j = 0; j<=i;j++){
                double part_1 = c1[i]*c1[j]+c2[i]*c2[j]+c3[i]*c3[j];
                double part_2 = c1[i]*c2[j]+c2[i]*c3[j]+c3[i]*c1[j];
                double part_3 = c1[j]*c2[i]+c2[j]*c3[i]+c3[j]*c1[i];
                w120[surface.label_of_triangle(k)].result_(i,j) += 1/18.*(part_1+part_2/2.+part_3/2.)*area;
            }
        }
    }

    return w120;
}

