#define GSLEIG_IMPLEMENTATION
#include "gsleig.h"
#include <assert.h>
#include <stdlib.h>
#include "Kmath.h"
#include <iostream>
#include "tensor.h"

EigensystemWorkspace::EigensystemWorkspace (int dim) {
    assert (dim > 0);
    dim_ = dim;
    priv_ = gsl_eigen_symmv_alloc (dim);
    privV_ = gsl_vector_alloc (dim);
    privM_ = gsl_matrix_alloc (dim, dim);
    privA_ = gsl_matrix_alloc (dim, dim);
    for (int i = 0; i != dim_; ++i)
    for (int j = 0; j != dim_; ++j)
        gsl_matrix_set (matr_ (), i, j, 1e99);
    if (!priv_ || !privV_ || !privM_ || !privA_) {
        std::cerr << "out of memory...\n";
        abort ();
    }
}

EigensystemWorkspace::~EigensystemWorkspace () {
    gsl_eigen_symmv_free (work_ ());
    gsl_vector_free (eigv_ ());
    gsl_matrix_free (vecs_ ());
    gsl_matrix_free (matr_ ());
}

template <typename SYMMETRIC_MATRIX>
void EigensystemWorkspace::diagonalize_matrix (const SYMMETRIC_MATRIX &m) {
    assert (m.size1 () == static_cast<unsigned int>(dim_));
    assert (m.size2 () == static_cast<unsigned int>(dim_));

    for (int i = 0; i != dim_; ++i)
    for (int j = 0; j <= i; ++j)
        gsl_matrix_set (matr_ (), i, j, m(i,j));

    gsl_eigen_symmv (matr_ (), eigv_ (), vecs_ (), work_ ());
    gsl_eigen_symmv_sort (eigv_ (), vecs_ (), GSL_EIGEN_SORT_ABS_ASC);
}

template void EigensystemWorkspace::diagonalize_matrix (const SymmetricMatrix <3> &);
template void EigensystemWorkspace::diagonalize_matrix (const SymmetricMatrix <6> &);

double EigensystemWorkspace::eigenvalue (int i) {
    assert (i >= 0);
    assert (i < dim_);
    double eval_i = gsl_vector_get (eigv_ (), i);
    return eval_i;
}

EigensystemWorkspace::vector_t EigensystemWorkspace::eigenvector (int i) {
    assert (i >= 0);
    assert (i < dim_);
    gsl_vector_view evec_i = gsl_matrix_column (vecs_ (), i);
    vector_t ret;
    for (int i = 0; i != dim_; ++i)
        ret[i] = gsl_vector_get (&evec_i.vector, i);
    return ret;
}
