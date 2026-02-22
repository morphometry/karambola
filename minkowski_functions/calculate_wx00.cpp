#include "calculate.h"
#include <cassert>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

CompWiseScalarMinkValResultType calculate_w000(const Triangulation& surface){
    CompWiseScalarMinkValResultType w000;
    //calculate w000
    for(unsigned int i = 0; i<surface.n_triangles();i++){
        double a = dot(surface.com_of_triangle(i),surface.normal_vector_of_triangle(i))*surface.area_of_triangle(i);
        w000[surface.label_of_triangle(i)].result_ += a * (1./3);
    }
    return w000;
}

CompWiseScalarMinkValResultType calculate_w100(const Triangulation& surface){
    CompWiseScalarMinkValResultType w100;
    //calculate w100
    for(unsigned int i = 0; i<surface.n_triangles();i++){
        w100[surface.label_of_triangle(i)].result_ += surface.area_of_triangle (i) * (1./3);
    }
    return w100;
}

CompWiseScalarMinkValResultType calculate_w200(const Triangulation& surface){
    CompWiseScalarMinkValResultType w200;
    //calculate w200
    for(unsigned int i = 0; i<surface.n_triangles();i++){
        for(unsigned int j = 0; j<3;j++){
            double local_value = 0;
            if(surface.ith_neighbour_of_triangle(i,j) != NEIGHBOUR_UNASSIGNED){
                Vector a = surface.normal_vector_of_triangle(surface.ith_neighbour_of_triangle(i,j));
                Vector b = surface.normal_vector_of_triangle(i);
                double c = dot(a,b)/(norm(a)*norm(b));
                assert (c <= 1.000001);
                if (c>= 1) c=1.;
                assert (c <= 1.);
                double alpha = acos(c);

                Vector convex = surface.com_of_triangle(i) + b - (surface.com_of_triangle(surface.ith_neighbour_of_triangle(i,j)) + a);
                Vector concave = surface.com_of_triangle(i) - b - (surface.com_of_triangle(surface.ith_neighbour_of_triangle(i,j)) - a);
                if(norm(convex) < norm(concave)) alpha = -alpha;

                local_value = alpha * surface.get_edge_length(i,j)/(2.*6.);
            }
            w200[surface.label_of_triangle(i)].result_ += local_value;
        }
    }
    return w200;
}

CompWiseScalarMinkValResultType calculate_w300(const Triangulation& surface){
    CompWiseScalarMinkValResultType w300;
    //calculate w300
    for(unsigned int i = 0; i<surface.n_vertices();i++){
        double sum = surface.sum_of_angles_of_ith_vertex(i);
        for (unsigned int j = 0; j< surface.get_triangles_of_vertex(i).size();j++){
            for(int k = 0; k < 3; k++){
                if(surface.ith_vertex_of_triangle(surface.get_triangles_of_vertex(i).at(j),k)==i){
                    double angle = surface.get_angle_of_ith_edge(surface.get_triangles_of_vertex(i).at(j),k);
                    double w3_part = (2*M_PI*(angle/sum))- angle;
                    w300[surface.label_of_triangle(surface.get_triangles_of_vertex(i).at(j))].result_ += w3_part/3.;
                }
            }
        }
    }
    return w300;
}


