#ifndef WRITE_FUNCTIONS
#define WRITE_FUNCTIONS

#include "../lib/common.h"
#include "../lib/CalcOptions.h"
#include "../lib/check_surface.h"
#include "../lib/MinkValResultType.h"
#include <math.h>
#include <cassert>
#include <iomanip>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include "print_explanations.h"

void write_surface_props_to_file(const CalcOptions& CO, const SurfaceStatistics& SurfStat);

void write_CompWiseEigenSystemMinkValResultType_to_file(const CalcOptions& CO, const CompWiseEigenSystemMinkValResultType &w_eigsys, bool append = false);

void write_CompWiseMatrixMinkValResultType_to_file( const CalcOptions& CO,
                                                    const CompWiseMatrixMinkValResultType &w,
                                                    bool append = false);

void write_CompWiseTensor3MinkValResultType_to_file( const CalcOptions& CO,
                                                     const CompWiseTensor3MinkValResultType &w);

void write_CompWiseTensor4MinkValResultType_to_file( const CalcOptions& CO,
                                                     const CompWiseTensor4MinkValResultType &w);

void write_CompWiseSphMinkMinkValResultType_to_file( const CalcOptions& CO,
                                                     const CompWiseSphMinkMinkValResultType &w,
                                                     bool append = false);

void write_CompWiseScalarMinkValResultType_to_file( const CalcOptions& CO,
                                                    const CompWiseScalarMinkValResultType &w000,
                                                    const CompWiseScalarMinkValResultType &w100,
                                                    const CompWiseScalarMinkValResultType &w200,
                                                    const CompWiseScalarMinkValResultType &w300,
                                                    bool append = false);



void write_CompWiseVectorMinkValResultType_to_file( const CalcOptions& CO,
                                                    const CompWiseVectorMinkValResultType &w010,
                                                    const CompWiseVectorMinkValResultType &w110,
                                                    const CompWiseVectorMinkValResultType &w210,
                                                    const CompWiseVectorMinkValResultType &w310,
                                                    bool append = false);

#endif // WRITE_FUNCTIONS
