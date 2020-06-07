#include "../karambola.h"
#include "test_compare_functions.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE( "polyreader ok" ){
    std::string infilename = "inputs/sphere.poly";
    std::cout << "testing:  " << infilename << std::endl;
    Triangulation surface;
    CalcOptions CO;
    std::ifstream is (infilename.c_str());
//    CO.labels_set = true;
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_poly_file (&adap2, is);
    CHECK (surface.n_triangles () == 4800);
}

TEST_CASE( "polyreader fails, poly file has no labels" ){
    std::string infilename = "inputs/sphere.poly";
    std::cout << "testing:  " << infilename << std::endl;
    Triangulation surface;
    CalcOptions CO;
    std::ifstream is (infilename.c_str());
//    CO.labels_set = true;
    ReadIntoTriangulation adap1 (&surface, true);
    TriangulatingPolyFileSink adap2 (&adap1);
    REQUIRE_THROWS(parse_poly_file (&adap2, is));
}
