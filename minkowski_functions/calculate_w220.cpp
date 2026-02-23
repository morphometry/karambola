// calculate W220,  integral dA  (1/6) (1/R1 + 1/R2)  r (tensor) r
#include "calculate.h"
#include "../lib/Kmath.h"
#include <cassert>

CompWiseMatrixMinkValResultType calculate_w220( const Triangulation& surface,
                                                const CompWiseScalarMinkValResultType& w200,
                                                const CompWiseVectorMinkValResultType& w210){
    //calculate w220
    CompWiseMatrixMinkValResultType w220;
    for (unsigned int l = 0; l<surface.n_triangles();l++){
        for (unsigned int k = 0; k<3;k++){
            if(surface.ith_neighbour_of_triangle(l,k) != NEIGHBOUR_UNASSIGNED){
                Vector a = surface.normal_vector_of_triangle(surface.ith_neighbour_of_triangle(l,k));
                Vector b = surface.normal_vector_of_triangle(l);
                double c = dot(a,b)/(norm(a)*norm(b));
                assert (c <= 1.000001);
                if (c>= 1) c=1.;
                double alpha = acos(c);

                Vector convex = surface.com_of_triangle(l) + b - (surface.com_of_triangle(surface.ith_neighbour_of_triangle(l,k)) + a);
                Vector concave = surface.com_of_triangle(l) - b - (surface.com_of_triangle(surface.ith_neighbour_of_triangle(l,k)) - a);
                if(norm(convex) < norm(concave)) alpha = -alpha;

                double e = surface.get_edge_length(l,k);

                Vector ref_vec(0,0,0);
                if (w200.size() != 0)
                    ref_vec = get_ref_vec(surface.label_of_triangle(l),w200,w210);

                Vector e_c1 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(l,k)) - ref_vec;
                Vector e_c2 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(l,(k+1)%3)) - ref_vec;
                for (unsigned int i = 0; i<3;i++){
                    for (unsigned int j = 0; j<=i;j++){
                        double w220_part = alpha*e*(e_c1[i] * e_c1[j] + 0.5 * ( e_c1[i] * e_c2[j] + e_c1[j] * e_c2[i] ) + e_c2[i] * e_c2[j]);
                        double local_value = w220_part/(18.*2.);
                        w220[surface.label_of_triangle(l)].result_(i,j) += local_value;
                    }
                }
            }
        }
    }

    return w220;
}

