// calculate W202,  integral dA  (1/6) (1/R1 + 1/R2)  n (tensor) n
#include "calculate.h"
#include <math.h>
#include <cassert>

CompWiseMatrixMinkValResultType calculate_w202(const Triangulation& surface){
    //calculate w202
    CompWiseMatrixMinkValResultType w202;
    for (unsigned int l = 0; l<surface.n_triangles();l++){
        for (unsigned int k = 0; k<3;k++){
            if(surface.ith_neighbour_of_triangle(l,k) != NEIGHBOUR_UNASSIGNED){
                Vector n2 = surface.normal_vector_of_triangle(surface.ith_neighbour_of_triangle(l,k));
                Vector n1 = surface.normal_vector_of_triangle(l);
                double c = dot(n1,n2)/(norm(n1)*norm(n2));
                assert (c <= 1.000001);
                if (c>= 1) c=1.;
                double alpha = acos(c);

                Vector convex = surface.com_of_triangle(l) + n1 - (surface.com_of_triangle(surface.ith_neighbour_of_triangle(l,k)) + n2);
                Vector concave = surface.com_of_triangle(l) - n1 - (surface.com_of_triangle(surface.ith_neighbour_of_triangle(l,k)) - n2);
                if(norm(convex) < norm(concave)) alpha = -alpha;

                double e_norm = surface.get_edge_length(l,k);
                Vector e_c1 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(l,k));
                Vector e_c2 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(l,(k+1)%3));

                Vector e = (e_c2 - e_c1)/e_norm;
                Vector n_a = (n1 + n2)/(norm(n1+n2));
                Vector n_i = cross_product(e,n_a);

                double local_value = 0;
                for (unsigned int i = 0; i<3;i++){
                    for (unsigned int j = 0; j<=i;j++){
                        local_value = e_norm*((alpha + sin(alpha))*n_a[i]*n_a[j]+(alpha - sin(alpha))*n_i[i]*n_i[j]);
                        //1/2. because every edge is calculated twice
                        w202[surface.label_of_triangle(l)].result_(i,j) += 1/12.*1/2.*local_value;
                    }
                }
            }
        }
    }
    return w202;
}

