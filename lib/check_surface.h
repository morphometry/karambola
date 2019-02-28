#ifndef CHECK_SURFACE
#define CHECK_SURFACE

#include "Triangulation.h"
#include "CalcOptions.h"

struct SurfaceStatistics
{
        //statistics
        double shortest_edge;
        double longest_edge;
        double smallest_area;
        double largest_area;
};

SurfaceStatistics check_surface(CalcOptions *CO, Triangulation& surface);


#endif // CHECK_SURFACE
