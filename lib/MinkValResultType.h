/** -*-c++-*-
 *
 *  Copyright 2009 Friedrich-Alexander Universitaet Erlangen-Nuernberg
 *
 *  MinkValResultType.h
 *
 *  This class 
 *    - provides datatypes to store scalar, tensorial and vectorial
 *      Minkowski functionals
 *
 *  Gerd Schroeder-Turk, August 2009
 *  Fabian Schaller, May 2010         modified output formats
 *
 */

#ifndef MINK_VAL_RESULT_TYPE
#define MINK_VAL_RESULT_TYPE


#include <iostream>
#include "Vector.h"
#include "tensor.h"
#include <map>
#include <vector>
#include <string>



class SphMink{
    public:
    std::vector < double > ql;
    std::vector < double > wl;
    private:
};
// ---------------------------------------------------
//
// Class to store a result. Definitions are provided 
// for
//         (a) T = double
//         (b) T = Vector 
//         (c) T = symmetric_matrix 
// 
template<class T>
class MinkValResultType
{
public:
  
  // true if no error has occured during computation
  bool computation_successful_;

  // variable that actually contains the result
  T result_;

  // name of quantity
  std::string name_;

  // brief single-word comments that are appended to the corresponding
  // result line in output file
  std::vector< std::string > keywords_;

  // comments printed in separate lines
  std::vector< std::string > comments_;

  // constructor
  MinkValResultType();

  // append keyword to keyword list
  void append_keyword( const std::string &keyword );

  void append_comment( const std::string &comment );

  // merge two result structures
  //void merge( const MinkValResultType<T> &RT2 );

private:

  static const std::string name_not_yet_assigned_;
};


// ---------------------------------------------------
//
//  define further combined data types
// 
typedef MinkValResultType< double > ScalarMinkValResultType;
typedef MinkValResultType< Vector > VectorMinkValResultType;
typedef MinkValResultType< symmetric_matrix > MatrixMinkValResultType;
typedef MinkValResultType< SymmetricRank4Tensor > Tensor4MinkValResultType;
typedef MinkValResultType< Rank3Tensor > Tensor3MinkValResultType;
typedef MinkValResultType< SphMink >     SphMinkMinkValResultType;


template <typename VALUE>
class CompWiseResultType : public std::map <unsigned int, MinkValResultType <VALUE> >
{
public:
    void add_contribution (unsigned label, double prefactor, const VALUE &val)
    {
        addmul ((*this)[label].result_, prefactor, val);
    }
    typename std::map <unsigned int,MinkValResultType<VALUE> >::iterator it;
};

/// structures for output of componentwise calcuation
typedef CompWiseResultType <double> CompWiseScalarMinkValResultType;
typedef CompWiseResultType <Vector> CompWiseVectorMinkValResultType;
typedef CompWiseResultType <symmetric_matrix> CompWiseMatrixMinkValResultType;
typedef CompWiseResultType <SymmetricRank4Tensor> CompWiseTensor4MinkValResultType;
typedef CompWiseResultType <Rank3Tensor> CompWiseTensor3MinkValResultType;
typedef CompWiseResultType <SphMink> CompWiseSphMinkMinkValResultType;

// structures for eigensystem output
class EigenSystem{
    public:
    std::vector < std::vector<double> > eigen_vectors;
    std::vector < double > eigen_values;
    private:
};

typedef MinkValResultType< EigenSystem >                    EigenSystemMinkValResultType;
typedef std::map<unsigned int,EigenSystemMinkValResultType> CompWiseEigenSystemMinkValResultType;




template<class T>
void MinkValResultType<T>::append_keyword( const std::string &keyword )
{
  for( std::vector<std::string>::iterator it=keywords_.begin();
       it!=keywords_.end(); it++                                 )
    if( *it == keyword )
      return;
  keywords_.push_back( keyword );
}    

template<class T>
void MinkValResultType<T>::append_comment( const std::string &comment )
{
  for( std::vector<std::string>::iterator it=comments_.begin();
       it!=comments_.end(); it++                                 )
    if( *it == comment )
      return;
  comments_.push_back( comment );
}    

#endif
