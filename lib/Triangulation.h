#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "Vector.h"
#include <vector>
#include <map>
#include "readpoly.h"
#include <iostream>
#include <cstdlib>
const int LABEL_UNASSIGNED = -300;
const int NEIGHBOUR_UNASSIGNED = -200;


class Vertex{
    public:
        Vector pos;
        std::vector<unsigned int> triangles;
        int number_in_file;
    private:
};


class Tri{
    public:
        int label;
        int vertices[3];
        int neighbour_triangles[3];
    private:
};


inline bool test_isnan(double value){
    return value != value;
}

class Triangulation {
    public:

        int append_vertex(double x , double y, double z, int number_in_file);

        void change_pos_of_vertex(unsigned int i, double x , double y, double z);
        void change_pos_of_vertex(unsigned int i, Vector pos);


        void append_triangle(unsigned int vert1, unsigned int vert2, unsigned int vert3, int plabel);

        //create lookup tables
        void create_vertex_polygon_lookup_table();
        void create_polygon_polygon_lookup_table();



        // Returns the area of ith triangle
        double area_of_triangle (int i) const {
            Vector a = Vertices.at(Triangles.at(i).vertices[1]).pos - Vertices.at(Triangles.at(i).vertices[0]).pos;
            Vector b = Vertices.at(Triangles.at(i).vertices[2]).pos - Vertices.at(Triangles.at(i).vertices[0]).pos;
            return norm(cross_product(a,b))/2.;
        }

        // Returns the label of ith triangle
        double label_of_triangle (int i) const {
            return Triangles.at(i).label;
        }

        //Returns center of mass of ith triangle
        Vector com_of_triangle (int i) const {
            Vector com = (Vertices.at(Triangles.at(i).vertices[0]).pos + Vertices.at(Triangles.at(i).vertices[1]).pos + Vertices.at(Triangles.at(i).vertices[2]).pos)/3.;
            return com;
        }


        //Returns the normal vector of ith triangle
        Vector normal_vector_of_triangle (int i) const {
            Vector a = Vertices.at(Triangles.at(i).vertices[1]).pos - Vertices.at(Triangles.at(i).vertices[0]).pos;
            Vector b = Vertices.at(Triangles.at(i).vertices[2]).pos - Vertices.at(Triangles.at(i).vertices[0]).pos;
            Vector normal = cross_product(a,b)/norm(cross_product(a,b));
            return normal;
        }





        //Return index of Triangle that is neigbour of triangle a across the polygon edge (i, i+1)
        int ith_neighbour_of_triangle (int a, int i) const {
            return Triangles.at(a).neighbour_triangles[i];
        }

        //Returns the ith vertex of triangle a
        unsigned int ith_vertex_of_triangle (int a, int i) const{
            return Triangles.at(a).vertices[i];
        }

        //edge length of ith edge of Triangle a
        double get_edge_length (int a, int i) const{
            return norm (Vertices.at(Triangles.at(a).vertices[i]).pos - Vertices.at(Triangles.at(a).vertices[(i+1)%3]).pos );
        }

        //returns the angle at the ith edgle of triangle a
        double get_angle_of_ith_edge(int a, int i) const {
            Vector aa = Vertices.at(Triangles.at(a).vertices[i]).pos - Vertices.at(Triangles.at(a).vertices[(i+1)%3]).pos;
            Vector bb = Vertices.at(Triangles.at(a).vertices[i]).pos - Vertices.at(Triangles.at(a).vertices[(i+2)%3]).pos;
            double c = dot(aa,bb)/(norm(aa)*norm(bb));
            if (c > 1)
                c = 1;
            double alpha = acos(c);
            if (test_isnan(alpha)){
                std::cout << "Error in Triangle " << a << " with label " << Triangles.at(a).label << ":" << std::endl;
                std::cout << "Vector aa  = Punkt " << Triangles.at(a).vertices[i] << " - Punkt " << Triangles.at(a).vertices[(i+1)%3] << std::endl;
                std::cout << "Vector bb  = Punkt " << Triangles.at(a).vertices[i] << " - Punkt " << Triangles.at(a).vertices[(i+2)%3] << std::endl;
                std::cout << "Vector aa " << aa << " = " << Vertices.at(Triangles.at(a).vertices[i]).pos << "-" << Vertices.at(Triangles.at(a).vertices[(i+1)%3]).pos << std::endl;
                std::cout << "Vector bb " << bb << " = " << Vertices.at(Triangles.at(a).vertices[i]).pos << "-" << Vertices.at(Triangles.at(a).vertices[(i+2)%3]).pos << std::endl;
                std::cout << "dot(aa,bb) = " << dot(aa,bb) << std::endl;
                std::cout << "norm(aa)*norm(bb) = " << norm(aa)*norm(bb) << std::endl;
                std::cout << "c = " << c << std::endl;
                std::exit(-1);
            }
            return alpha;
        }

        //returns the sum of the angles which adjoin vertex i
        double sum_of_angles_of_ith_vertex(unsigned int i) const{
            double sum = 0;
            for (unsigned int j = 0; j< get_triangles_of_vertex(i).size();j++){
                for(unsigned int k = 0; k < 3; k++){
                    if(ith_vertex_of_triangle(get_triangles_of_vertex(i).at(j),k)==i)
                        sum += get_angle_of_ith_edge(get_triangles_of_vertex(i).at(j),k);
                }
            }
            return sum;
        }

        //returns the number of triangles
        unsigned int n_triangles() const {
            return Triangles.size();
        }

        //returns the number of vertices
        unsigned int n_vertices() const {
            return Vertices.size();
        }

        //returns the triangles whose adjoin vertex a
        std::vector<unsigned int> get_triangles_of_vertex(int a) const {
            return Vertices.at(a).triangles;
        }

        //returns the coordinates of vertex a
        Vector get_pos_of_vertex(int a) const {
            return Vertices.at(a).pos;
        }

        //returns the original vertex number in poly file
        int get_origial_number_of_vertex_in_file(int a) const {
            return Vertices.at(a).number_in_file;
        }

    private:
        std::vector<Tri> Triangles;
        std::vector<Vertex> Vertices;
};

class ReadIntoTriangulation : public PolyFileSink
{
public:
    ReadIntoTriangulation (Triangulation *target, bool read_with_labels)
    {
        with_labels_ = read_with_labels;
        tri_ = target;
    }

    virtual long insert_vertex (double x, double y, double z, const prop_list_t &props) {
        return tri_->append_vertex (x, y, z, props.object_number);
    }

    virtual void insert_facet (const vertex_id_list_t &vs, const prop_list_t &props) {
        int lab = LABEL_UNASSIGNED;
        assert (vs.size () == 3u);
        if (with_labels_)
            lab = props.label_from_alpha_value ();
        tri_->append_triangle (vs[0], vs[1], vs[2], lab);
    }

private:
    bool with_labels_;
    Triangulation *tri_;
};


#endif
