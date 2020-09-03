#include "provided.h"
#include "support.h"

using namespace std;

double calcf_N(GeoCoord a, GeoCoord b, GeoCoord c)
{
    double f_N = calcg_N(a, b) + calch_N(b, c);
    return f_N;
}

double calcg_N(GeoCoord a, GeoCoord b)
{
    return distanceEarthMiles(a, b);
}

double calch_N(GeoCoord b, GeoCoord c)
{
    return distanceEarthMiles(b, c);
}
