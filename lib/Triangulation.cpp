#include "Triangulation.h"




int Triangulation::append_vertex(double x , double y, double z){
    int ret = Vertices.size ();
    Vertex vert;
    vert.pos[0] = x;
    vert.pos[1] = y;
    vert.pos[2] = z;
    Vertices.push_back(vert);
    return ret;
}

void Triangulation::change_pos_of_vertex(unsigned int i, double x , double y, double z){
    if(i >= Vertices.size () ){
        std::cerr << "can't change pos of vertex, vertex number " << i << " too high!" << std::endl;
        exit(-1);
    }
    Vertices.at(i).pos[0] = x;
    Vertices.at(i).pos[1] = y;
    Vertices.at(i).pos[2] = z;
}
void Triangulation::change_pos_of_vertex(unsigned int i, Vector pos){
    if(i >= Vertices.size () ){
        std::cerr << "can't change pos of vertex, vertex number " << i << " too high!" << std::endl;
        exit(-1);
    }
    Vertices.at(i).pos = pos;
}



void Triangulation::append_triangle(unsigned int vert1, unsigned int vert2, unsigned int vert3, int plabel){
    Tri polygon;
    polygon.label = plabel;
    polygon.vertices[0] = vert1;
    polygon.vertices[1] = vert2;
    polygon.vertices[2] = vert3;
    Triangles.push_back(polygon);
}

void Triangulation::create_vertex_polygon_lookup_table(){
    for(unsigned int i = 0; i<n_triangles();i++){
        for(int j = 0; j<3;j++){
            Vertices.at(Triangles.at(i).vertices[j]).triangles.push_back(i);
        }
    }
}

void Triangulation::create_polygon_polygon_lookup_table(){
    for (unsigned int i = 0; i<n_triangles();i++) {
        for (unsigned int j = 0; j<3; j++){
            Triangles.at(i).neighbour_triangles[j] = NEIGHBOUR_UNASSIGNED;
            for(unsigned int k = 0; k < Vertices.at(Triangles.at(i).vertices[j]).triangles.size();k++){
                unsigned int k_tri =    Vertices.at(Triangles.at(i).vertices[j]).triangles.at(k);
                for(unsigned int l = 0; l < 3; l++){
                    if(Triangles.at(i).vertices[(j+1)%3] == Triangles.at(k_tri).vertices[l] && k_tri != i)
                        Triangles.at(i).neighbour_triangles[j] = k_tri;
                }
            }
        }
    }
}

