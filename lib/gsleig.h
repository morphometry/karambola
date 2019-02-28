#ifndef GSLEIG_HPP_INCLUDED
#define GSLEIG_HPP_INCLUDED

#include "Vector.h"
#ifdef GSLEIG_IMPLEMENTATION
#include <gsl/gsl_eigen.h>
#endif
#include "common.h"

class EigensystemWorkspace
    : public noncopyable {

public:
    typedef Vector vector_t;

    EigensystemWorkspace (int dim = 3);
    ~EigensystemWorkspace ();

    // 1. call this
    template <typename SYMMETRIC_MATRIX>
    void diagonalize_matrix (const SYMMETRIC_MATRIX &);
    // 2. retrieve results
    double   eigenvalue (int i);
    vector_t eigenvector (int i);

private:
#ifdef GSLEIG_IMPLEMENTATION
    gsl_eigen_symmv_workspace *work_ () {
        return static_cast <gsl_eigen_symmv_workspace *> (priv_); }
    gsl_matrix *vecs_ () { return static_cast <gsl_matrix *> (privM_); }
    gsl_vector *eigv_ () { return static_cast <gsl_vector *> (privV_); }
    gsl_matrix *matr_ () { return static_cast <gsl_matrix *> (privA_); }
#endif
    void *priv_, *privM_, *privV_, *privA_;
    int dim_;
};

#endif // GSLEIG_HPP_INCLUDED
