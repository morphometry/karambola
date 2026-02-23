#include "calculate.h"
#include "../lib/Kmath.h"
#include <cassert>

CompWiseTensor3MinkValResultType calculate_w103(const Triangulation& surface){

    CompWiseTensor3MinkValResultType w103;

    //calculate w103
    for (unsigned int l = 0; l < surface.n_triangles(); l++){
        double area = surface.area_of_triangle(l);
        Vector n = surface.normal_vector_of_triangle(l);
        for (unsigned int i = 0; i<3;i++){
            for (unsigned int j = 0; j<3;j++){
                for (unsigned int k = 0; k<3;k++){
                    w103[surface.label_of_triangle(l)].result_(i,j,k) += 1/3.*area*n[i]*n[j]*n[k];
                }
            }
        }
    }

    return w103;
}

