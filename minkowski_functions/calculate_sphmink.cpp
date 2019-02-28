#include "calculate.h"
#include <math.h>
#include <cassert>
#include <cstring>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_specfunc.h>

#define MAX_L 8

static
double sign (double x)
{
    if (x<0.)
        return -1.;
    else
        return 1.;
}

class SphericalMinkowskis
{
    gsl_complex d_[91];
    double total_area;

    const gsl_complex *d (int l) const {
        return const_cast <SphericalMinkowskis *> (this)->d (l);
    }

    gsl_complex *d (int l) {
        switch (l)
        {
        case 0:
            return d_;
        case 1:
            return d_+1;
        case 2:
            return d_+3;
        case 3:
            return d_+6;
        case 4:
            return d_+10;
        case 5:
            return d_+15;
        case 6:
            return d_+21;
        case 7:
            return d_+28;
        case 8:
            return d_+36;
        case 9:
            return d_+45;
        case 10:
            return d_+55;
        case 11:
            return d_+66;
        case 12:
            return d_+78;
        case 13:
            return d_+91;
        default:
            std::cerr << "MAX_L broken\n";
            std::abort ();
        }
    }

public:
    SphericalMinkowskis () {
        std::memset (d_, 0, sizeof d_);
        total_area = 0.;
    }

    void add_facet (Vector f)
    {
        double area = norm (f);
        f /= area;
        double cos_th = f[2];
        double phi = atan2 (f[1], f[0]);
        total_area += area;
        gsl_complex *dp = d (0);
        for (int l = 0; l <= MAX_L; ++l)
        {
            double l_prefactor = sqrt (4*M_PI/(2*l+1));
            assert (d (l) == dp);
            for (int m = 0; m <= l; ++m, ++dp)
            {
                double leg = gsl_sf_legendre_sphPlm (l, m, cos_th);
                leg *= area;
                leg *= l_prefactor;
                gsl_complex ylm = gsl_complex_polar (leg, m*phi);
                *dp = gsl_complex_add (*dp, ylm);
            }
        }
    }

    double ql (int l) const
    {
        assert (l <= MAX_L);
        assert (l >= 0);
        const gsl_complex *pe = d (l+1);
        const gsl_complex *p = d (l);
        double r = gsl_complex_abs2 (*p);
        for (++p; p != pe; ++p)
            r += 2 * gsl_complex_abs2 (*p);
        return sqrt (r) / total_area;
    }

    double wl (int l) const
    {
        assert (l <= MAX_L);
        assert (l >= 0);
        gsl_complex v = {0., 0.};
        const gsl_complex *p = d (l);
        int ma, mb, mc;
        for (ma = -l; ma <= l; ++ma)
        for (mb = -l; mb <= l; ++mb)
        for (mc = -l; mc <= l; ++mc)
        {
            gsl_complex q = gsl_complex_polar (gsl_sf_coupling_3j (2*l, 2*l, 2*l, 2*ma, 2*mb, 2*mc), 0);

            // get the proper Clm coefficients from the arrays.
            // this is complicated a bit by the fact that we don't store the negative-m
            // coefficients, which are complex conjugates of the +m ones.
            gsl_complex a;
            if (ma < 0) {
                a = p[-ma];
                a = gsl_complex_conjugate (a);
                if ((-ma) % 2)
                    a = gsl_complex_mul_real (a, -1);
            }
            else
            {
                a = p[ma];
            }
            q = gsl_complex_mul (q, a);

            if (mb < 0) {
                a = p[-mb];
                a = gsl_complex_conjugate (a);
                if ((-mb) % 2)
                    a = gsl_complex_mul_real (a, -1);
            }
            else
            {
                a = p[mb];
            }
            q = gsl_complex_mul (q, a);

            if (mc < 0) {
                a = p[-mc];
                a = gsl_complex_conjugate (a);
                if ((-mc) % 2)
                    a = gsl_complex_mul_real (a, -1);
            }
            else
            {
                a = p[mc];
            }
            q = gsl_complex_mul (q, a);

            v = gsl_complex_add (v, q);
        }

        if (fabs (GSL_IMAG (v)) > .1e-4)
            std::cerr << "large spurious imaginary component (l = "
                      << l << GSL_REAL (v) << ", " << GSL_IMAG (v) << "i)\n";
        return pow (gsl_complex_abs (v), 1./3) / total_area * sign (GSL_REAL (v));
    }
};




CompWiseSphMinkMinkValResultType calculate_sphmink(const Triangulation& surface){

    CompWiseSphMinkMinkValResultType sphmink;


    //calculate sphmink
    std::map <unsigned int, SphericalMinkowskis> data;
    for (unsigned int k = 0; k < surface.n_triangles(); k++){
        double area = surface.area_of_triangle(k);
        Vector n = surface.normal_vector_of_triangle(k);
        data[surface.label_of_triangle(k)].add_facet (n*area);
    }

    std::map <unsigned int, SphericalMinkowskis>::const_iterator it;
    for (it = data.begin (); it != data.end (); ++it)
    {
        for (int l = 0; l <= MAX_L; ++l){
            sphmink[it->first].result_.ql.at(l) += it->second.ql (l);
            sphmink[it->first].result_.wl.at(l) += it->second.wl (l);
        }
    }

    return sphmink;
}

