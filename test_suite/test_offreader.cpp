#include "../karambola.h"
#include "test_compare_functions.hpp"
#include "catch.hpp"
#include <fstream>
using Catch::Matchers::Contains;

TEST_CASE( "parse valid off file without labels" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid.off");
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_off_file (&adap2, is);
    CHECK (surface.n_triangles () == 6 * 2);
    CHECK (surface.n_vertices () == 8);
    CHECK_THAT(surface.get_pos_of_vertex (0), IsApprox({1, 0, 1}, 1e-12));
    CHECK_THAT(surface.get_pos_of_vertex (7), IsApprox({0, -1, -1}, 1e-12));
    for (int i = 0; i != 12; ++i)
        CHECK (surface.label_of_triangle(i) == LABEL_UNASSIGNED);
}

TEST_CASE( "parse valid off file without labels, but expecting to find some" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid.off");
    ReadIntoTriangulation adap1 (&surface, true);
    TriangulatingPolyFileSink adap2 (&adap1);
    REQUIRE_THROWS(parse_off_file (&adap2, is), Contains("no attribute c for facet 0"));
}

TEST_CASE( "parse valid off file without labels but with vertex colors" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-vertexColors.off");
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_off_file (&adap2, is);
    CHECK (surface.n_triangles () == 6 * 2);
    CHECK (surface.n_vertices () == 8);
    CHECK_THAT(surface.get_pos_of_vertex (0), IsApprox({1, 0, 1}, 1e-12));
    CHECK_THAT(surface.get_pos_of_vertex (7), IsApprox({0, -1, -1}, 1e-12));
    for (int i = 0; i != 12; ++i)
        CHECK (surface.label_of_triangle(i) == LABEL_UNASSIGNED);
}

TEST_CASE( "parse invalid off file without labels, references nonexisting vertex" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-wrongVertexIndexInFacet.off");
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    REQUIRE_THROWS_WITH(parse_off_file (&adap2, is), Contains("file refers to vertex 8 which I do not know (vertex_map size = 8)"));
}

TEST_CASE( "parse invalid truncated file" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-truncated.off");
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    REQUIRE_THROWS_WITH(parse_off_file (&adap2, is), Contains("Cannot read vertex ids for facet 2: Expected number, got <linebreak>."));
}

TEST_CASE( "parse invalid truncated file, missing z coordinate" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-truncated2.off");
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    REQUIRE_THROWS_WITH(parse_off_file (&adap2, is), Contains("Expected number, got <linebreak>."));
}

TEST_CASE( "parse invalid truncated file 3" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-truncated3.off");
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    REQUIRE_THROWS(parse_off_file (&adap2, is));
}

TEST_CASE( "parse invalid off file with bad magic" ){
    Triangulation surface;
    std::istringstream is ("POLY\nnot an off!");
    ReadIntoTriangulation adap1 (&surface, true);
    TriangulatingPolyFileSink adap2 (&adap1);
    REQUIRE_THROWS(parse_off_file (&adap2, is), Contains("format error in .off file: expected OFF, got POLY"));
}

TEST_CASE( "parse valid off file without labels but with colors" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-colorsNoLabels.off");
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_off_file (&adap2, is);
    CHECK (surface.n_triangles () == 6 * 2);
    CHECK (surface.n_vertices () == 8);
    CHECK_THAT(surface.get_pos_of_vertex (0), IsApprox({1, 0, 1}, 1e-12));
    CHECK_THAT(surface.get_pos_of_vertex (7), IsApprox({0, -1, -1}, 1e-12));
    for (int i = 0; i != 12; ++i)
        CHECK (surface.label_of_triangle(i) == LABEL_UNASSIGNED);
}

TEST_CASE( "parse valid off file with labels" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-labels.off");
    ReadIntoTriangulation adap1 (&surface, true);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_off_file (&adap2, is);
    CHECK (surface.n_triangles () == 6 * 2);
    CHECK (surface.n_vertices () == 8);
    CHECK_THAT(surface.get_pos_of_vertex (0), IsApprox({1, 0, 1}, 1e-12));
    CHECK_THAT(surface.get_pos_of_vertex (7), IsApprox({0, -1, -1}, 1e-12));
    for (int i = 0; i != 6; ++i)
        CHECK (surface.label_of_triangle(i) == 1);
    for (int i = 6; i != 12; ++i)
        CHECK (surface.label_of_triangle(i) == 2);
}

TEST_CASE( "parse valid off file with labels, and some tab charaters" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-labelsTabs.off");
    ReadIntoTriangulation adap1 (&surface, true);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_off_file (&adap2, is);
    CHECK (surface.n_triangles () == 6 * 2);
    CHECK (surface.n_vertices () == 8);
    CHECK_THAT(surface.get_pos_of_vertex (0), IsApprox({1, 0, 1}, 1e-12));
    CHECK_THAT(surface.get_pos_of_vertex (7), IsApprox({0, -1, -1}, 1e-12));
    for (int i = 0; i != 6; ++i)
        CHECK (surface.label_of_triangle(i) == 1);
    for (int i = 6; i != 12; ++i)
        CHECK (surface.label_of_triangle(i) == 2);
}

TEST_CASE( "parse valid off file with labels, ignoring them" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-labels.off");
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_off_file (&adap2, is);
    CHECK (surface.n_triangles () == 6 * 2);
    CHECK (surface.n_vertices () == 8);
    CHECK_THAT(surface.get_pos_of_vertex (0), IsApprox({1, 0, 1}, 1e-12));
    CHECK_THAT(surface.get_pos_of_vertex (7), IsApprox({0, -1, -1}, 1e-12));
    for (int i = 0; i != 12; ++i)
        CHECK (surface.label_of_triangle(i) == LABEL_UNASSIGNED);
}

TEST_CASE( "parse valid off file with labels, with DOS line endings" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-dosLineEndings.off");
    ReadIntoTriangulation adap1 (&surface, true);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_off_file (&adap2, is);
    CHECK (surface.n_triangles () == 6 * 2);
    CHECK (surface.n_vertices () == 8);
    CHECK_THAT(surface.get_pos_of_vertex (0), IsApprox({1, 0, 1}, 1e-12));
    CHECK_THAT(surface.get_pos_of_vertex (7), IsApprox({0, -1, -1}, 1e-12));
    for (int i = 0; i != 6; ++i)
        CHECK (surface.label_of_triangle(i) == 1);
    for (int i = 6; i != 12; ++i)
        CHECK (surface.label_of_triangle(i) == 2);
}

TEST_CASE( "parse valid off file with labels and comments" ){
    Triangulation surface;
    std::ifstream is ("inputs/cuboid-labelsWithComments.off");
    ReadIntoTriangulation adap1 (&surface, true);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_off_file (&adap2, is);
    CHECK (surface.n_triangles () == 6 * 2);
    CHECK (surface.n_vertices () == 8);
    CHECK_THAT(surface.get_pos_of_vertex (0), IsApprox({1, 0, 1}, 1e-12));
    CHECK_THAT(surface.get_pos_of_vertex (7), IsApprox({0, -1, -1}, 1e-12));
    for (int i = 0; i != 6; ++i)
        CHECK (surface.label_of_triangle(i) == 1);
    for (int i = 6; i != 12; ++i)
        CHECK (surface.label_of_triangle(i) == 2);
}

