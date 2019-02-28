#include "calculate.h"
#include <math.h>
#include <cassert>

CompWiseTensor4MinkValResultType calculate_w104 (const Triangulation& surface) {

    CompWiseTensor4MinkValResultType w104;

    for (unsigned int k = 0; k < surface.n_triangles(); k++){
        double area = surface.area_of_triangle(k);
        Vector n = surface.normal_vector_of_triangle(k);
        SymmetricRank4Tensor t = fourth_tensorial_power (n);
        w104.add_contribution (surface.label_of_triangle(k), 1./3 * area, t);
    }

    return w104;
}
