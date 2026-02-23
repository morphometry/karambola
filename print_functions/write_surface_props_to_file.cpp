#include "write_functions.h"

#include "../lib/Kmath.h"
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <filesystem>


void write_surface_props_to_file(const CalcOptions& CO, const SurfaceStatistics& SurfStat){

    //write messages to file
    std::ofstream wfile;

    std::filesystem::create_directories(CO.outfoldername);

    std::string filename = CO.outfoldername + "/" + "surface_props";
    wfile.open(filename.c_str());

    wfile << "shortest edge = " << SurfStat.shortest_edge << std::endl;
    wfile << "longest edge  = " << SurfStat.longest_edge << std::endl;
    wfile << "smallest area = " << SurfStat.smallest_area << std::endl;
    wfile << "largest area  = " << SurfStat.largest_area << std::endl;
    wfile << std::endl;

    int sw = 15;

    wfile << std::setw(sw) << "label" << std::setw(sw) << "  " << std::endl;

    std::map<int,int>::const_iterator it_label;
    for ( it_label = CO.labeled_surfaces_closed.begin() ; it_label != CO.labeled_surfaces_closed.end(); it_label++ ){
        if ( (*it_label).first == -300)
            wfile << std::setw(sw)<< "ALL";
        else
            wfile << std::setw(sw)<< (*it_label).first;
        wfile << std::setw(sw);
        if ((*it_label).second == 0) wfile << "closed" << std::endl;
        if ((*it_label).second == 1) wfile << "shared" << std::endl;
        if ((*it_label).second == 2) wfile << "open"  << std::endl;

    }

    wfile.close();

}


