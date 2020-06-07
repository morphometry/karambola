#include "../karambola.h"
#include "test_compare_functions.hpp"
#include "catch.hpp"
#include <fstream>

TEST_CASE( "polyreader ok" ){
    Triangulation surface;
    std::ifstream is ("inputs/sphere.poly");
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_poly_file (&adap2, is);
    CHECK (surface.n_triangles () == 4800);
}

TEST_CASE( "polyreader fails, poly file has no labels" ){
    Triangulation surface;
    std::ifstream is ("inputs/sphere.poly");
    ReadIntoTriangulation adap1 (&surface, true);
    TriangulatingPolyFileSink adap2 (&adap1);
    REQUIRE_THROWS(parse_poly_file (&adap2, is));
}
