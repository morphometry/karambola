#ifndef TENSOR_RANK4_INCLUDED 
#define TENSOR_RANK4_INCLUDED 

#include <assert.h>
#include "Vector.h"
#include <vector>

template <int N>
class SymmetricMatrix
{
public:
    typedef SymmetricMatrix <N> this_t;
    static const int NUM_INDEP_ELEMENTS = N*(N+1)/2;

    SymmetricMatrix () {
        for (int i = 0; i != NUM_INDEP_ELEMENTS; ++i)
            st_[i] = 0.;
    }

    const double &operator() (int i, int j) const
    {
        return const_cast <this_t &> (*this) (i,j);
    }

    double &operator() (int i, int j)
    {
        if (i > j)
            return (*this)(j, i);
        assert (i < N && i >= 0);
        assert (j < N && j >= 0);
        return st_[j*(j+1)/2+i];
    }

    size_t size1 () const { return N; }
    size_t size2 () const { return N; }

    friend void addmul (this_t &lhs, double pref, const this_t &rhs) {
        for (int i = 0; i != NUM_INDEP_ELEMENTS; ++i)
            lhs.st_[i] += pref * rhs.st_[i];
    }

    this_t &operator+= (const this_t &rhs) {
        for (int i = 0; i != NUM_INDEP_ELEMENTS; ++i)
            st_[i] += rhs.st_[i];
        return *this;
    }

    this_t &operator-= (const this_t &rhs) {
        for (int i = 0; i != NUM_INDEP_ELEMENTS; ++i)
            st_[i] -= rhs.st_[i];
        return *this;
    }


private:
    double st_[NUM_INDEP_ELEMENTS];
};


typedef SymmetricMatrix <3> symmetric_matrix;

// store a symmetric rank-4 tensor as a 6x6 symmetric matrix.
// the type initializes itself to zero in the default constructor.
//
// elem(0,0)   is T_xxxx
// elem(1,0)   is T_yyxx
// elem(3,0)   is T_yzxx * sqrt 2
// elem(4,5)   is T_xzxy * 2
// elem(5,5)   is T_xyxy * 2

struct SymmetricRank4Tensor : public SymmetricMatrix <6>
{
};

SymmetricRank4Tensor fourth_tensorial_power (const Vector &);
template <int N>
std::vector <double> eigenvalues (const SymmetricMatrix <N> &);



//Quick and dirty Rank3 Tensor

class Rank3Tensor{
    public:
    Rank3Tensor () {
        d_[0][0][0] = d_[0][1][0] = d_[0][2][0] = d_[1][0][0] = d_[1][1][0] = d_[1][2][0] = d_[2][0][0] = d_[2][1][0] = d_[2][2][0] = 0.;
        d_[0][0][1] = d_[0][1][1] = d_[0][2][1] = d_[1][0][1] = d_[1][1][1] = d_[1][2][1] = d_[2][0][1] = d_[2][1][1] = d_[2][2][1] = 0.;
        d_[0][0][2] = d_[0][1][2] = d_[0][2][2] = d_[1][0][2] = d_[1][1][2] = d_[1][2][2] = d_[2][0][2] = d_[2][1][2] = d_[2][2][2] = 0.;
    }

//    Rank3Tensor (double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22) {
//        d_[0][0] = m00;
//        d_[0][1] = m01;
//        d_[0][2] = m02;
//        d_[1][0] = m10;
//        d_[1][1] = m11;
//        d_[1][2] = m12;
//        d_[2][0] = m20;
//        d_[2][1] = m21;
//        d_[2][2] = m22;
//    }

    //
    // accessors
    //

    double &operator() (int i, int j, int k) {
        return d_[i][j][k];
    }

    double operator() (int i, int j, int k) const {
        return d_[i][j][k];
    }

private:
    double d_[3][3][3];


};



#endif /* TENSOR_RANK4_INCLUDED */
