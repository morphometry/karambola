/** -*-c++-*-
 *
 *  Copyright 2009 Friedrich-Alexander Universitaet Erlangen-Nuernberg
 *
 *  MinkValResultType.C
 *
 *  Gerd Schroeder-Turk, August 2009
 *
 */


#include "MinkValResultType.h"

// ---------------------------------------------------
//
// Spezializations of constructor for 
// 
//         (a) T = double
//         (b) T = symmetric_matrix 
//         (c) T = Vector 
// 

template <class T>
const std::string MinkValResultType< T >::name_not_yet_assigned_ = "name_not_yet_assigned";

template <>
MinkValResultType< SymmetricRank4Tensor >::MinkValResultType()
{
    assert (result_(0,3) == 0.);
    computation_successful_ = true;
    name_ = name_not_yet_assigned_;
}

template <>
MinkValResultType< Rank3Tensor >::MinkValResultType()
{
    for (int x=0; x<3; ++x)
    for (int y=0; y<3; ++y)
    for (int z=0; z<3; ++z)
        result_(x,y,z) = 0;
    computation_successful_ = true;
    name_ = name_not_yet_assigned_;
}

template <>
MinkValResultType< Vector >::MinkValResultType()
{
    Vector zeroVector(0,0,0);
    result_ = zeroVector;
    computation_successful_ = true;
    name_ = name_not_yet_assigned_;
}

template <>
MinkValResultType< symmetric_matrix >::MinkValResultType()
{
    symmetric_matrix zeroMatrix;
    for (int x=0; x<3; ++x)
    for (int y=0; y<3; ++y)
        zeroMatrix(x,y)=0;

    result_ = zeroMatrix;
    computation_successful_ = true;
    name_ = name_not_yet_assigned_;
}

template <>
MinkValResultType< double >::MinkValResultType()
{
    result_ = 0;
    computation_successful_ = true;
    name_ = name_not_yet_assigned_;
}

template <>
MinkValResultType< EigenSystem >::MinkValResultType()
{
    std::vector<double> zeroVector(3);
    for (int x=0; x<3; ++x) zeroVector.at(x)=0;
    std::vector < std::vector<double> > zero_eigen_vectors(3,zeroVector);

    EigenSystem zeroEigSys;
    zeroEigSys.eigen_vectors = zero_eigen_vectors;
    zeroEigSys.eigen_values = zeroVector;

    result_ = zeroEigSys;
    computation_successful_ = true;
    name_ = name_not_yet_assigned_;
}

template <>
MinkValResultType< SphMink >::MinkValResultType()
{
    std::vector < double > zero_ql(12,0);

    SphMink zeroSphMink;
    zeroSphMink.ql = zero_ql;
    zeroSphMink.wl = zero_ql;

    result_ = zeroSphMink;
    computation_successful_ = true;
    name_ = name_not_yet_assigned_;
}

