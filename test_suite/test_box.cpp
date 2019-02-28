#include "../karambola.h"
#include "test_compare_functions.hpp"
#include "catch.hpp"
#include <sstream>

TEST_CASE( "Box" )
{
    std::string infilename = "inputs/box_a=2_b=3_c=4.poly";
    const double a = 2;
    const double b = 3;
    const double c = 4;
    double pi = M_PI;
    std::cout << "testing:  " << infilename << std::endl;
    CompWiseScalarMinkValResultType w000_ref, w100_ref, w200_ref, w300_ref;
    CompWiseVectorMinkValResultType w010_ref, w110_ref, w210_ref, w310_ref;

    Triangulation surface;
    CalcOptions CO;
    std::ifstream is (infilename.c_str());
    CO.labels_set = false;
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_poly_file (&adap2, is);

    surface.create_vertex_polygon_lookup_table();
    surface.create_polygon_polygon_lookup_table();

    check_surface(&CO, surface);

    CHECK (calculate_w000(surface)[LABEL_UNASSIGNED].result_ == Approx(a*b*c).epsilon(1e-4));
    CHECK (calculate_w100(surface)[LABEL_UNASSIGNED].result_ == Approx(2/3.*(a*b+a*c+b*c)).epsilon(1e-4));
    CHECK (calculate_w200(surface)[LABEL_UNASSIGNED].result_ == Approx(pi/3.*(a+b+c)).epsilon(1e-4));
    CHECK (calculate_w300(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.).epsilon(1e-4));
    CHECK_THAT(calculate_w010(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0},1e-3));
    CHECK_THAT(calculate_w110(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0},1e-3));
    CHECK_THAT(calculate_w210(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0},1e-3));
    CHECK_THAT(calculate_w310(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0},1e-3));
    CHECK_THAT(Vector (1/12.*pow(a,3)*b*c, 1/12.*pow(b,3)*a*c, 1/12.*pow(c,3)*a*b),
               EigvalIsApprox(calculate_w020(surface),LABEL_UNASSIGNED,1e-4));
    CHECK_THAT(Vector (1/6.*(1/3.*pow(a,3)*(b+c)+pow(a,2)*b*c), 1/6.*(1/3.*pow(b,3)*(a+c)+pow(b,2)*a*c), 1/6.*(1/3.*pow(c,3)*(a+b)+pow(c,2)*a*b)),
               EigvalIsApprox(calculate_w120(surface),LABEL_UNASSIGNED,1e-4));
    CHECK_THAT(Vector (pi/36.*(pow(a,3)+3*pow(a,2)*(b+c)), pi/36.*(pow(b,3)+3*pow(b,2)*(a+c)), pi/36.*(pow(c,3)+3*pow(c,2)*(a+b))),
               EigvalIsApprox(calculate_w220(surface),LABEL_UNASSIGNED,1e-4));
    CHECK_THAT(Vector(pi/3.*pow(a,2), pi/3.*pow(b,2), pi/3.*pow(c,2)),
               EigvalIsApprox(calculate_w320(surface),LABEL_UNASSIGNED,1e-4));
    CHECK_THAT(Vector(2/3.*a*b, 2/3.*a*c, 2/3.*b*c),
               EigvalIsApprox(calculate_w102(surface),LABEL_UNASSIGNED,1e-4));
    CHECK_THAT(Vector(pi/6.*(a+b), pi/6.*(a+c), pi/6.*(b+c)),
               EigvalIsApprox(calculate_w202(surface),LABEL_UNASSIGNED,1e-4));
}

TEST_CASE( "schiefe Box" )
{
    std::string infilename = "inputs/schiefebox_a=2_b=3_c=4.poly";
    const double a = 2;
    const double b = 3;
    const double c = 4;
    double pi = M_PI;
    std::cout << "testing:  " << infilename << std::endl;
    CompWiseScalarMinkValResultType w000_ref, w100_ref, w200_ref, w300_ref;
    CompWiseVectorMinkValResultType w010_ref, w110_ref, w210_ref, w310_ref;

    Triangulation surface;
    CalcOptions CO;
    std::ifstream is (infilename.c_str());
    CO.labels_set = false;
    ReadIntoTriangulation adap1 (&surface, false);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_poly_file (&adap2, is);

    surface.create_vertex_polygon_lookup_table();
    surface.create_polygon_polygon_lookup_table();

    check_surface(&CO, surface);

    CHECK (calculate_w000(surface)[LABEL_UNASSIGNED].result_ == Approx(a*b*c).epsilon(1e-4));
    CHECK (calculate_w100(surface)[LABEL_UNASSIGNED].result_ == Approx(2/3.*(a*b+a*c+b*c)).epsilon(1e-4));
    CHECK (calculate_w200(surface)[LABEL_UNASSIGNED].result_ == Approx(pi/3.*(a+b+c)).epsilon(1e-4));
    CHECK (calculate_w300(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.).epsilon(1e-4));
    CHECK_THAT(calculate_w010(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0},1e-3));
    CHECK_THAT(calculate_w110(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0},1e-3));
    CHECK_THAT(calculate_w210(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0},1e-3));
    CHECK_THAT(calculate_w310(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0},1e-3));
    CHECK_THAT(Vector (1/12.*pow(a,3)*b*c, 1/12.*pow(b,3)*a*c, 1/12.*pow(c,3)*a*b),
               EigvalIsApprox(calculate_w020(surface),LABEL_UNASSIGNED,5e-3));
    CHECK_THAT(Vector (1/6.*(1/3.*pow(a,3)*(b+c)+pow(a,2)*b*c), 1/6.*(1/3.*pow(b,3)*(a+c)+pow(b,2)*a*c), 1/6.*(1/3.*pow(c,3)*(a+b)+pow(c,2)*a*b)),
               EigvalIsApprox(calculate_w120(surface),LABEL_UNASSIGNED,1e-4));
    CHECK_THAT(Vector (pi/36.*(pow(a,3)+3*pow(a,2)*(b+c)), pi/36.*(pow(b,3)+3*pow(b,2)*(a+c)), pi/36.*(pow(c,3)+3*pow(c,2)*(a+b))),
               EigvalIsApprox(calculate_w220(surface),LABEL_UNASSIGNED,1e-4));
    CHECK_THAT(Vector(pi/3.*pow(a,2), pi/3.*pow(b,2), pi/3.*pow(c,2)),
               EigvalIsApprox(calculate_w320(surface),LABEL_UNASSIGNED,1e-4));
    CHECK_THAT(Vector(2/3.*a*b, 2/3.*a*c, 2/3.*b*c),
               EigvalIsApprox(calculate_w102(surface),LABEL_UNASSIGNED,1e-4));
    CHECK_THAT(Vector(pi/6.*(a+b), pi/6.*(a+c), pi/6.*(b+c)),
               EigvalIsApprox(calculate_w202(surface),LABEL_UNASSIGNED,1e-4));
}


