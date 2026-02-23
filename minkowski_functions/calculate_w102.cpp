#include "calculate.h"
#include "../lib/Kmath.h"
#include <cassert>

CompWiseMatrixMinkValResultType calculate_w102(const Triangulation& surface){

    CompWiseMatrixMinkValResultType w102;

    //calculate w102
    for (unsigned int k = 0; k < surface.n_triangles(); k++){
        double area = surface.area_of_triangle(k);
        Vector n = surface.normal_vector_of_triangle(k);
        for (unsigned int i = 0; i<3;i++){
            for (unsigned int j = 0; j<=i;j++){
                w102[surface.label_of_triangle(k)].result_(i,j) += 1/3.*area*n[i]*n[j];
            }
        }
    }

    return w102;
}

