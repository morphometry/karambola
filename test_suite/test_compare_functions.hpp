#pragma once
#include "catch.hpp"
#include <sstream>


// The matcher class
class VecApprox : public Catch::MatcherBase<Vector> {
    Vector m_ref;
    double m_tolerance;
public:
    VecApprox( Vector ref, double tolerance ) : m_ref( ref ), m_tolerance(tolerance) {}
    VecApprox( double ref, double tolerance ) : m_tolerance(tolerance) {m_ref[0]= ref;m_ref[1]= ref;m_ref[2]= ref;}

    // Performs the test for this matcher
    virtual bool match( Vector const& w ) const override {
      bool equal = true;
      for (int i = 0; i < 3; i++){
        if (std::abs(w[i] - m_ref[i]) > m_tolerance)
          equal = false;
      }
      return equal;
    }

    // Produces a string describing what this matcher does.
    std::string describe() const override {
        std::ostringstream ss;
        ss << "is approx " << m_ref << " with tol " << m_tolerance;
        return ss.str();
    }
};

// The builder function
inline VecApprox IsApprox( Vector w, double tolerance = 1e-6) {
    return VecApprox( w, tolerance);
}
inline VecApprox IsApprox( double val, double tolerance = 1e-6) {
    return VecApprox( val, tolerance);
}

inline VecApprox EigvalIsApprox( CompWiseMatrixMinkValResultType w, int label = LABEL_UNASSIGNED, double tolerance = 1e-6) {
    Vector Vec_w_eig(calculate_eigensystem(w)[label].result_.eigen_values);
    return VecApprox( Vec_w_eig, tolerance);
}






//inline void compare_Vector(Vector w, Vector ref, double tolerance = 1e-3){
//    for (int i = 0; i < 3; i++){
//      if (ref[i] == 0)
//        CHECK (w[i] == Approx(ref[i]).margin(tolerance));
//      else  
//        CHECK (w[i] == Approx(ref[i]).epsilon(tolerance));
//    }
//}
//inline void compare_Vector(Vector w, double ref, double tolerance = 1e-3){
//    for (int i = 0; i < 3; i++){label
//      if (ref == 0)
//        CHECK (w[i] == Approx(ref).margin(tolerance));
//      else  
//        CHECK (w[i] == Approx(ref).epsilon(tolerance));
//    }
//}
inline bool compare_Vector(Vector w, Vector ref, double tolerance = 1e-3){
    bool equal = true;
    for (int i = 0; i < 3; i++){
      if (std::abs(w[i] - ref[i]) > tolerance)
        equal = false;
    }
    return equal;
}
inline bool compare_Vector(Vector w, double ref, double tolerance = 1e-3){
    bool equal = true;
    for (int i = 0; i < 3; i++){
      if (std::abs(w[i] - ref) > tolerance)
        equal = false;
    }
    return equal;
}
//inline void compare_Vector(std::vector<double> w, Vector ref){
//    for (int i = 0; i < 3; i++){
//      if (ref[i] == 0)
//        CHECK (w[i] == Approx(ref[i]).margin(1e-3));
//      else  
//        CHECK (w[i] == Approx(ref[i]).epsilon(1e-3));
//    }
//}
inline bool compare_Eigval(CompWiseMatrixMinkValResultType w, Vector ref, int label, double tolerance = 1e-3){
    std::vector<double> eig_vals = calculate_eigensystem(w)[label].result_.eigen_values;
    bool equal = true;
    for (int i = 0; i < 3; i++){
      if (std::abs(eig_vals[i] - ref[i]) > tolerance)
        equal = false;
    }
    return equal;
}
inline bool compare_Eigval(CompWiseMatrixMinkValResultType w, double ref, int label, double tolerance = 1e-3){
    std::vector<double> eig_vals = calculate_eigensystem(w)[label].result_.eigen_values;
    bool equal = true;
    for (int i = 0; i < 3; i++){
      if (std::abs(eig_vals[i] - ref) > tolerance)
        equal = false;
    }
    return equal;
}

