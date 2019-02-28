#include "tensor.h"
#include "gsleig.h"

SymmetricRank4Tensor fourth_tensorial_power (const Vector &v)
{
    const double sqrt2 = sqrt (2.);
    const double x = v[0], y = v[1], z = v[2];
    const double t[6] = { x*x, y*y, z*z, y*z*sqrt2, x*z*sqrt2, x*y*sqrt2 };
    SymmetricRank4Tensor ret;
    for (int i = 0; i != 6; ++i)
    for (int j = 0; j <= i; ++j)
        ret(i,j) = t[i]*t[j];
    return ret;
}

template <int N>
std::vector <double> eigenvalues (const SymmetricMatrix <N> &mat)
{
    EigensystemWorkspace e (N);
    e.diagonalize_matrix (mat);
    std::vector <double> ret;
    ret.reserve (N);
    for (int i = 0; i != N; ++i)
        ret.push_back (e.eigenvalue (i));
    return ret;
}

template std::vector <double> eigenvalues (const SymmetricMatrix <3> &mat);
template std::vector <double> eigenvalues (const SymmetricMatrix <6> &mat);
