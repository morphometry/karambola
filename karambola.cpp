// vim: et:sw=4:ts=4
//!  -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//!  Authors       : Fabian Schaller     fabian.schaller@physik.uni-erlangen.de
//!                  Sebastian Kapfer    sebastian.kapfer@physik.uni-erlangen.de
//!                  Gerd Schroeder-Turk gerd.schroeder-turk@physik.uni-erlangen.de
//!  File          : karambola.cpp
//!  copyright     : Universitaet Erlangen
//!  -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#include "karambola.h"
#include <sstream>

int main( int argc, char *argv[] ){
  

    //create CalcOptions object
    CalcOptions CO;
    SurfaceStatistics SurfStat;

    //create Triangulation object
    Triangulation surface;

    //check command line for inputs
    check_command_line(&CO, argc, argv);

    std::cout << "Converting Polyfile to Minkowski triangulation format ...";
    std::cout.flush();
    std::ifstream is (CO.infilename.c_str());

    ReadIntoTriangulation adap1 (&surface, CO.labels_set);
    TriangulatingPolyFileSink adap2 (&adap1);
    parse_poly_file (&adap2, is);


    //lookup_tables are needed for the minkowski calculations
    surface.create_vertex_polygon_lookup_table();
    surface.create_polygon_polygon_lookup_table();
    std::cout << "     done" << std::endl;



    //check surface properties (if surface is closed, shared, open, statistics, ...)
    std::cout << "check surface ..." << std::endl;
    SurfStat = check_surface(&CO, surface);

    std::cout << "shortest edge = " << SurfStat.shortest_edge << std::endl;
    std::cout << "longest edge  = " << SurfStat.longest_edge << std::endl;
    std::cout << "smallest area = " << SurfStat.smallest_area << std::endl;
    std::cout << "largest area  = " << SurfStat.largest_area << std::endl;
    std::cout << "                       done" << std::endl;




    //scalars
    std::cout << "calculate w000 ..." << std::flush;
    CompWiseScalarMinkValResultType w000 = calculate_wxxx<CompWiseScalarMinkValResultType>("w000", CO, surface);
    std::cout << "done" << std::endl;

    std::cout << "calculate w100 ..." << std::flush;
    CompWiseScalarMinkValResultType w100
        = calculate_wxxx<CompWiseScalarMinkValResultType>("w100", CO, surface);
    std::cout << "done" << std::endl;

    std::cout << "calculate w200 ..." << std::flush;
    CompWiseScalarMinkValResultType w200 = calculate_wxxx<CompWiseScalarMinkValResultType>("w200", CO, surface);
    std::cout << "done" << std::endl;

    std::cout << "calculate w300 ..." << std::flush;
    CompWiseScalarMinkValResultType w300 = calculate_wxxx<CompWiseScalarMinkValResultType>("w300", CO, surface);
    std::cout << "done" << std::endl;

    //Vectors
    std::cout << "calculate w010 ..." << std::flush;
    CompWiseVectorMinkValResultType w010
        = calculate_wxxx<CompWiseVectorMinkValResultType>("w010", CO, surface);
    std::cout << "done" << std::endl;

    std::cout << "calculate w110 ..." << std::flush;
    CompWiseVectorMinkValResultType w110 = calculate_wxxx<CompWiseVectorMinkValResultType>("w110", CO, surface);
    std::cout << "done" << std::endl;

    std::cout << "calculate w210 ..." << std::flush;
    CompWiseVectorMinkValResultType w210 = calculate_wxxx<CompWiseVectorMinkValResultType>("w210", CO, surface);
    std::cout << "done" << std::endl;

    std::cout << "calculate w310 ..." << std::flush;
    CompWiseVectorMinkValResultType w310 = calculate_wxxx<CompWiseVectorMinkValResultType>("w310", CO, surface);
    std::cout << "done" << std::endl;


    //Matrix
    std::cout << "calculate w020 ..." << std::flush;
    CompWiseMatrixMinkValResultType w020
        = calculate_wxxx<CompWiseMatrixMinkValResultType>("w020", CO, surface, w000, w010);
    std::cout << "done" << std::endl;

    std::cout << "calculate w120 ..." << std::flush;
    CompWiseMatrixMinkValResultType w120
        = calculate_wxxx<CompWiseMatrixMinkValResultType>("w120", CO, surface, w100, w110);
    std::cout << "done" << std::endl;

    std::cout << "calculate w220 ..." << std::flush;
    CompWiseMatrixMinkValResultType w220
        = calculate_wxxx<CompWiseMatrixMinkValResultType>("w220", CO, surface, w200, w210);
    std::cout << "done" << std::endl;

    std::cout << "calculate w320 ..." << std::flush;
    CompWiseMatrixMinkValResultType w320
        = calculate_wxxx<CompWiseMatrixMinkValResultType>("w320", CO, surface, w300, w310);
    std::cout << "done" << std::endl;

    std::cout << "calculate w102 ..." << std::flush;
    CompWiseMatrixMinkValResultType w102
        = calculate_wxxx<CompWiseMatrixMinkValResultType>("w102", CO, surface);
    std::cout << "done" << std::endl;

    std::cout << "calculate w202 ..." << std::flush;
    CompWiseMatrixMinkValResultType w202
        = calculate_wxxx<CompWiseMatrixMinkValResultType>("w202", CO, surface);
    std::cout << "done" << std::endl;

    // Tensor Rank 4
    CompWiseTensor4MinkValResultType w104
        = calculate_wxxx<CompWiseTensor4MinkValResultType>("w104", CO, surface);


    // Tensor Rank 3
    CompWiseTensor3MinkValResultType w103
        = calculate_wxxx<CompWiseTensor3MinkValResultType>("w103", CO, surface);


    //sphmink
    std::cout << "calculate msm ..." << std::flush;
    CompWiseSphMinkMinkValResultType sphmink
        = calculate_wxxx<CompWiseSphMinkMinkValResultType>("msm", CO, surface);
    std::cout << "done" << std::endl;


    std::cout << std::endl;




    //print results in terminal

    /*
       print_CompWiseScalarMinkValResultType(w000);
       print_CompWiseScalarMinkValResultType(w100);
       print_CompWiseScalarMinkValResultType(w200);
       print_CompWiseScalarMinkValResultType(w300);

       print_CompWiseVectorMinkValResultType(w010);
       print_CompWiseVectorMinkValResultType(w110);
       print_CompWiseVectorMinkValResultType(w210);
       print_CompWiseVectorMinkValResultType(w310);

       print_CompWiseMatrixMinkValResultType(w020);
       print_CompWiseMatrixMinkValResultType(w120);
       print_CompWiseMatrixMinkValResultType(w220);
       print_CompWiseMatrixMinkValResultType(w320);
       print_CompWiseMatrixMinkValResultType(w102);
       print_CompWiseMatrixMinkValResultType(w202);
       */

    //write results to file
    std::cout << "write results to files ...";
    std::cout.flush();
    write_surface_props_to_file(CO, SurfStat);
    write_CompWiseScalarMinkValResultType_to_file(CO, w000,w100,w200,w300);
    write_CompWiseVectorMinkValResultType_to_file(CO, w010,w110,w210,w310);
    write_CompWiseMatrixMinkValResultType_to_file(CO, w020);
    write_CompWiseMatrixMinkValResultType_to_file(CO, w120);
    write_CompWiseMatrixMinkValResultType_to_file(CO, w220);
    write_CompWiseMatrixMinkValResultType_to_file(CO, w320);
    write_CompWiseMatrixMinkValResultType_to_file(CO, w102);
    write_CompWiseMatrixMinkValResultType_to_file(CO, w202);
    write_CompWiseTensor3MinkValResultType_to_file(CO, w103);
    write_CompWiseTensor4MinkValResultType_to_file(CO, w104);
    write_CompWiseSphMinkMinkValResultType_to_file(CO, sphmink);
    std::cout << "     done" <<std::endl;

    //calculate eigenvalues and eigensystems
    std::cout << "calculate and write eigensystems to file ...";
    std::cout.flush();

    CompWiseEigenSystemMinkValResultType w020_eigsys;
    w020_eigsys = calculate_eigensystem(w020);
    write_CompWiseEigenSystemMinkValResultType_to_file(CO, w020_eigsys);

    CompWiseEigenSystemMinkValResultType w120_eigsys;
    w120_eigsys = calculate_eigensystem(w120);
    write_CompWiseEigenSystemMinkValResultType_to_file(CO, w120_eigsys);

    CompWiseEigenSystemMinkValResultType w220_eigsys;
    w220_eigsys = calculate_eigensystem(w220);
    write_CompWiseEigenSystemMinkValResultType_to_file(CO, w220_eigsys);

    CompWiseEigenSystemMinkValResultType w320_eigsys;
    w320_eigsys = calculate_eigensystem(w320);
    write_CompWiseEigenSystemMinkValResultType_to_file(CO, w320_eigsys);

    CompWiseEigenSystemMinkValResultType w102_eigsys;
    w102_eigsys = calculate_eigensystem(w102);
    write_CompWiseEigenSystemMinkValResultType_to_file(CO, w102_eigsys);

    CompWiseEigenSystemMinkValResultType w202_eigsys;
    w202_eigsys = calculate_eigensystem(w202);
    write_CompWiseEigenSystemMinkValResultType_to_file(CO, w202_eigsys);
    std::cout << "     done"<< std::endl;
}
