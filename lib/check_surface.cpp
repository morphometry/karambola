#include "check_surface.h"

#include "Kmath.h"
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <errno.h>
#include <stdexcept>


SurfaceStatistics check_surface(CalcOptions* CO, Triangulation& surface){


    SurfaceStatistics SurfStat;

    //are there polys/triangles?
    if (surface.n_triangles() == 0)
        throw std::runtime_error ("There are no polygons in your .poly file");

    //is surface closed?
    for(unsigned int i = 0; i<surface.n_triangles();i++){
        CO->create_label (surface.label_of_triangle (i), 0);
    }

    for(unsigned int i = 0; i<surface.n_triangles();i++){
      for(int j = 0; j < 3; j++){
        if(surface.ith_neighbour_of_triangle(i,j) != NEIGHBOUR_UNASSIGNED){
          if(surface.label_of_triangle(i) != surface.label_of_triangle(surface.ith_neighbour_of_triangle(i,j))){
            CO->create_label (surface.label_of_triangle (i), 1);
          }
        }
      }
    }
    for(unsigned int i = 0; i<surface.n_triangles();i++){
      for(int j = 0; j < 3; j++){
        if(surface.ith_neighbour_of_triangle(i,j) == NEIGHBOUR_UNASSIGNED){
            CO->create_label (surface.label_of_triangle (i), 2);
        }
      }
    }

    //get surface properties
    double shortest_edge = std::numeric_limits<double>::max();
    double longest_edge = 0;
    for(unsigned int i = 0; i<surface.n_triangles();i++){
        for(unsigned int j = 0; j<3;j++){
            Vector v1 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(i,j));
            Vector v2 = surface.get_pos_of_vertex(surface.ith_vertex_of_triangle(i,(j+1)%3));
            double e = norm(v1-v2);
            if (e<shortest_edge)
                shortest_edge = e;
            if (e>longest_edge)
                longest_edge = e;
        }
    }
    SurfStat.shortest_edge = shortest_edge;
    SurfStat.longest_edge = longest_edge;
    double smallest_area = std::numeric_limits<double>::max();
    double largest_area = 0;
    for(unsigned int i = 0; i<surface.n_triangles();i++){
        double area = surface.area_of_triangle(i);
        if (area<smallest_area)
                smallest_area = area;
        if (area>largest_area)
                largest_area = area;
    }
    SurfStat.smallest_area = smallest_area;
    SurfStat.largest_area = largest_area;
    if(smallest_area / largest_area < 1e-12){
        std::cout << std::endl;
        std::cout << "WARNING: there is something wrong with the area of your facets which could cause troubles with the facet normals!" << std::endl;
        std::cout << "smallest area: " << smallest_area << std::endl;
        std::cout << "largest area:  " << largest_area << std::endl;
        std::cout << std::endl;
    }

    //check if more than one objects are at one vertex
    for(unsigned int i = 0; i<surface.n_vertices();i++){
        unsigned int sum_of_triangles = 1;
        if (surface.get_triangles_of_vertex(i).size() == 0) continue;
        unsigned int start_triangle = surface.get_triangles_of_vertex(i).at(0);
        unsigned int old_triangle = NEIGHBOUR_UNASSIGNED;
        unsigned int triangle = surface.get_triangles_of_vertex(i).at(0);
        unsigned int new_triangle = surface.get_triangles_of_vertex(i).at(0);

        bool neigh_un     = false;
        bool normal_wrong = false;

        for (unsigned int j = 0; j< surface.get_triangles_of_vertex(i).size();j++){
            triangle = new_triangle;
            unsigned int vertex_id = 0;
            for (unsigned int k = 0; k < 3; k++){
                if(surface.ith_vertex_of_triangle(triangle,k)==i){
                    vertex_id = k;
                }
            }

            if (surface.ith_neighbour_of_triangle(triangle,vertex_id) == NEIGHBOUR_UNASSIGNED){ neigh_un = true; break;}
            new_triangle = surface.ith_neighbour_of_triangle(triangle,vertex_id);

            if (old_triangle == new_triangle){normal_wrong = true; break;}

            if (new_triangle == start_triangle) break;
            sum_of_triangles+=1;
            old_triangle = triangle;
        }

        if (normal_wrong == true){
            std::stringstream errMsg;
            errMsg << "shortest edge = " << SurfStat.shortest_edge << "\n";
            errMsg << "longest edge  = " << SurfStat.longest_edge << "\n";
            errMsg << "smallest area = " << SurfStat.smallest_area << "\n";
            errMsg << "largest area  = " << SurfStat.largest_area << "\n";
            errMsg << "your polyfile is damaged\n" << "your normals are messed up!!" << "\n";
            errMsg << "error occurs at vertex: " << surface.get_origial_number_of_vertex_in_file(i) << "\n";
//            errMsg << "and triangles " << triangle << " and " << old_triangle << "\n";
            throw std::runtime_error (errMsg.str().c_str());
        }

        if(surface.get_triangles_of_vertex(i).size() != sum_of_triangles && neigh_un == false){
            std::stringstream errMsg;
            errMsg << "your polyfile is damaged\n" << "there are more than one objects at vertex " <<  surface.get_origial_number_of_vertex_in_file(i) << ": " << surface.get_pos_of_vertex(i) << "\n";
            errMsg << "neighbour unassigned = " <<  neigh_un << "\n";
            errMsg << "surface.get_triangles_of_vertex(i).size() = " << surface.get_triangles_of_vertex(i).size()<< "\n";
            errMsg << "sum_of_triangles = " << sum_of_triangles << "\n";
            throw std::runtime_error (errMsg.str().c_str());
        }
    }

    return SurfStat;

}


