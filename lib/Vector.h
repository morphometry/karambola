#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED


#include <assert.h>
#include <math.h>
#include <ostream>
#include <iomanip>
#include <vector>

#include "./common.h"

class Vector {
public:
    Vector () {
        d_[0] = d_[1] = d_[2] = 0.;
    }

    Vector (double x_, double y_, double z_) {
        d_[0] = x_;
        d_[1] = y_;
        d_[2] = z_;
    }
    Vector (std::vector<double> v) {
        d_[0] = v.at(0);
        d_[1] = v.at(1);
        d_[2] = v.at(2);
    }

    //
    // accessors
    //

    double &operator[] (int i) {
        assert (i == 2 || i == 1 || i == 0);
        return d_[i];
    }

    double operator[] (int i) const {
        assert (i == 2 || i == 1 || i == 0);
        return d_[i];
    }

    //
    // operators
    //

    Vector const &operator+= (const Vector &rhs) {
        d_[0] += rhs[0];
        d_[1] += rhs[1];
        d_[2] += rhs[2];
        return *this;
    }

    Vector const &operator-= (const Vector &rhs) {
        d_[0] -= rhs[0];
        d_[1] -= rhs[1];
        d_[2] -= rhs[2];
        return *this;
    }

    Vector const &operator*= (double rhs) {
        d_[0] *= rhs;
        d_[1] *= rhs;
        d_[2] *= rhs;
        return *this;
    }

    Vector &operator/= (double rhs) {
        d_[0] /= rhs;
        d_[1] /= rhs;
        d_[2] /= rhs;
        return *this;
    }

private:
    double d_[3];
};

inline Vector operator+ (const Vector &lhs, const Vector &rhs) {
    return Vector (lhs[0]+rhs[0], lhs[1]+rhs[1], lhs[2]+rhs[2]);
}

inline Vector operator- (const Vector &lhs, const Vector &rhs) {
    return Vector (lhs[0]-rhs[0], lhs[1]-rhs[1], lhs[2]-rhs[2]);
}

inline Vector operator* (double f, const Vector &v) {
    return Vector (f*v[0], f*v[1], f*v[2]);
}

inline Vector operator* (const Vector &v, double f) {
    return f * v;
}

inline Vector operator/ (const Vector &v, double f) {
    return Vector (v[0]/f, v[1]/f, v[2]/f);
}

inline std::ostream &operator<< (std::ostream &lhs, const Vector &rhs) {
    lhs << std::setprecision(12) << "(" << rhs[0] << ", " << rhs[1] << " , "<< rhs[2] <<")";
    return lhs;
}

inline bool operator== (const Vector &lhs, const Vector &rhs) {
    return lhs[0] == rhs[0] and lhs[1] == rhs[1] and lhs[2] == rhs[2];
}

// standard euclidean 2-norm
inline double norm_sq (const Vector &v) {
    return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}

inline double norm (const Vector &v) {
    return sqrt (norm_sq (v));
}

// dot product
inline double dot (const Vector &lhs, const Vector &rhs) {
    return lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2];
}

inline bool vector_isnan (const Vector &x) {
    return is_nan (x[0]) || is_nan (x[1]) || is_nan (x[2]);
}

inline void vector_normalize (Vector *x) {
    *x *= norm (*x);
}

// cross product
inline Vector cross_product (const Vector &lhs, const Vector &rhs){
    Vector a;
    a[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1];
    a[1] = lhs[2]*rhs[0] - lhs[0]*rhs[2];
    a[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];
    return a;
}

#endif // VECTOR_H_INCLUDED
