#ifndef PRINT_EXPLANATIONS
#define PRINT_EXPLANATIONS

#include <fstream>
#include <string>
#include <iomanip>

extern const std::string version;
extern const std::pair<std::string, std::string> calc_forced;
extern const std::pair<std::string, std::string> cant_calc;
extern const std::pair<std::string, std::string> reference_origin_string;
extern const std::pair<std::string, std::string> reference_centroid_string;

void print_explanations(std::ofstream &wfile, std::string infilename);

#endif // PRINT_EXPLANATIONS
