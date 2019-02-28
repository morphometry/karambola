#include "../karambola.h"
#include "test_compare_functions.hpp"
#include "catch.hpp"
#include <sstream>

TEST_CASE( "Sphere" )
{
    std::string infilename = "inputs/Kugel_R=2_im_Ursprung.poly";
    const double R = 2;
    const double pi = M_PI;
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

    CHECK (calculate_w000(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.*pow(R,3)).epsilon(1e-2));
    CHECK (calculate_w100(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.*pow(R,2)).epsilon(1e-2));
    CHECK (calculate_w200(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.*pow(R,1)).epsilon(1e-2));
    CHECK (calculate_w300(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.*pow(R,0)).epsilon(1e-2));
    CHECK_THAT(calculate_w010(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0}));
    CHECK_THAT(calculate_w110(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0}));
    CHECK_THAT(calculate_w210(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0}));
    CHECK_THAT(calculate_w310(surface)[LABEL_UNASSIGNED].result_, IsApprox({0,0,0}));
    CHECK (compare_Eigval (calculate_w020(surface),
       4*pi/15.*pow(R,5),
       LABEL_UNASSIGNED, 0.5));
    CHECK (compare_Eigval (calculate_w120(surface),
       4*pi/9.*pow(R,4),
       LABEL_UNASSIGNED, 0.5));
    CHECK (compare_Eigval (calculate_w220(surface),
       4*pi/9.*pow(R,3),
       LABEL_UNASSIGNED, 1e-2));
    CHECK (compare_Eigval (calculate_w320(surface),
       4*pi/9.*pow(R,2),
       LABEL_UNASSIGNED, 1e-2));
    CHECK (compare_Eigval (calculate_w102(surface),
       4*pi/9.*pow(R,2),
       LABEL_UNASSIGNED, 1e-2));
    CHECK (compare_Eigval (calculate_w202(surface),
       4*pi/9.*pow(R,1),
       LABEL_UNASSIGNED, 1e-2));
}

TEST_CASE( "Sphere2" )
{
    std::string infilename = "inputs/sphere.poly";
    const double R = 0.5;
    const double pi = M_PI;
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

    CHECK (calculate_w000(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.*pow(R,3)).epsilon(1e-2));
    CHECK (calculate_w100(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.*pow(R,2)).epsilon(1e-2));
    CHECK (calculate_w200(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.*pow(R,1)).epsilon(1e-2));
    CHECK (calculate_w300(surface)[LABEL_UNASSIGNED].result_ == Approx(4*pi/3.*pow(R,0)).epsilon(1e-2));
    CHECK_THAT(calculate_w010(surface)[LABEL_UNASSIGNED].result_, IsApprox(0.5*4*pi/3.*pow(R,3),1e-2));
    CHECK_THAT(calculate_w110(surface)[LABEL_UNASSIGNED].result_, IsApprox(0.5*4*pi/3.*pow(R,2),1e-2));
    CHECK_THAT(calculate_w210(surface)[LABEL_UNASSIGNED].result_, IsApprox(0.5*4*pi/3.*pow(R,1),1e-2));
    CHECK_THAT(calculate_w310(surface)[LABEL_UNASSIGNED].result_, IsApprox(0.5*4*pi/3.*pow(R,0),1e-2));
    CHECK (compare_Eigval (calculate_w020(surface,calculate_w000(surface),calculate_w010(surface)),
       4*pi/15.*pow(R,5),
       LABEL_UNASSIGNED, 1e-2));
    CHECK (compare_Eigval (calculate_w120(surface,calculate_w100(surface),calculate_w110(surface)),
       4*pi/9.*pow(R,4),
       LABEL_UNASSIGNED, 1e-2));
    CHECK (compare_Eigval (calculate_w220(surface,calculate_w200(surface),calculate_w210(surface)),
       4*pi/9.*pow(R,3),
       LABEL_UNASSIGNED, 1e-2));
    CHECK (compare_Eigval (calculate_w320(surface,calculate_w300(surface),calculate_w310(surface)),
       4*pi/9.*pow(R,2),
       LABEL_UNASSIGNED, 1e-2));
    CHECK (compare_Eigval (calculate_w102(surface),
       4*pi/9.*pow(R,2),
       LABEL_UNASSIGNED, 1e-2));
    CHECK (compare_Eigval (calculate_w202(surface),
       4*pi/9.*pow(R,1),
       LABEL_UNASSIGNED));

}
