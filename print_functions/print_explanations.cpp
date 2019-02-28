#include "print_explanations.h"

#define STRINGIFY(s) STRINGIFY2(s)
#define STRINGIFY2(s) #s

const std::string version = "karambola " STRINGIFY(VERSION_NUMBER);
const std::pair<std::string, std::string> calc_forced ( "calculation_forced", "stubbornly applied the formulae even though the surface is shared or open");
const std::pair<std::string, std::string> cant_calc ( "cant_be_calculated", "can't be calculated because surface is shared or open");
const std::pair<std::string, std::string> reference_origin_string ( "reference_origin", "the tensors are calculated with respect to the origin");
const std::pair<std::string, std::string> reference_centroid_string ( "reference_centroid", "the tensors are calculated with respect to the minkowski-vectors");

void print_explanations(std::ofstream &wfile, std::string infilename){

	wfile << "# input_file = " << infilename << std::endl;
	wfile << "#" << std::endl;

	wfile << "# keyword explanations: (not all keywords have to show up)" << std::endl;

	wfile << "# closed = the labeled surface is closed" << std::endl;
	wfile << "# shared = the labeled surface has edges whose neighbours are surfaces with other labels" << std::endl;
	wfile << "# open   = the labeled surface is open, there are no neighbours to some of the edges" << std::endl;

	wfile << "# " << calc_forced.first << " = " << calc_forced.second << std::endl;
	wfile << "# " << cant_calc.first << " = " << cant_calc.second << std::endl;
	wfile << "# " << reference_origin_string.first << "   = " << reference_origin_string.second << std::endl;
	wfile << "# " << reference_centroid_string.first << " = " << reference_centroid_string.second << std::endl;

	wfile << "#" << std::endl;
	wfile << "# calculated with " << version << std::endl;
	wfile << "# contact: " << "karambola@physik.uni-erlangen.de" << std::endl;


}
