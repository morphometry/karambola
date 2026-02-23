#include "calculate.h"
#include "../lib/Kmath.h"
#include <cassert>

CompWiseVectorMinkValResultType calculate_w110(const Triangulation& surface){
    CompWiseVectorMinkValResultType w110;
    //calculate w110
    for (unsigned int i = 0; i<3;i++){
        for (unsigned int j = 0; j<surface.n_triangles();j++){
            double w110_part = surface.com_of_triangle(j)[i]*surface.area_of_triangle(j);
            w110[surface.label_of_triangle(j)].result_[i] += w110_part/3.;
        }
    }
    return w110;
}

