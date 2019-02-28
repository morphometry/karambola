#ifndef CALCULATE_H_INCLUDED
#define CALCULATE_H_INCLUDED

#include "../lib/MinkValResultType.h"
#include "../lib/Triangulation.h"
#include "../lib/CalcOptions.h"
#include "../lib/Vector.h"




CompWiseScalarMinkValResultType calculate_w000(const Triangulation& surface);
CompWiseScalarMinkValResultType calculate_w100(const Triangulation& surface);
CompWiseScalarMinkValResultType calculate_w200(const Triangulation& surface);
CompWiseScalarMinkValResultType calculate_w300(const Triangulation& surface);


CompWiseVectorMinkValResultType calculate_w010(const Triangulation& surface);
CompWiseVectorMinkValResultType calculate_w110(const Triangulation& surface);
CompWiseVectorMinkValResultType calculate_w210(const Triangulation& surface);
CompWiseVectorMinkValResultType calculate_w310(const Triangulation& surface);

CompWiseMatrixMinkValResultType calculate_w020( const Triangulation& surface,
                                                const CompWiseScalarMinkValResultType& w000 = CompWiseScalarMinkValResultType(),
                                                const CompWiseVectorMinkValResultType& w010 = CompWiseVectorMinkValResultType());

CompWiseMatrixMinkValResultType calculate_w120( const Triangulation& surface,
                                                const CompWiseScalarMinkValResultType& w100 = CompWiseScalarMinkValResultType(),
                                                const CompWiseVectorMinkValResultType& w110 = CompWiseVectorMinkValResultType());

CompWiseMatrixMinkValResultType calculate_w220( const Triangulation& surface,
                                                const CompWiseScalarMinkValResultType& w200 = CompWiseScalarMinkValResultType(),
                                                const CompWiseVectorMinkValResultType& w210 = CompWiseVectorMinkValResultType());

CompWiseMatrixMinkValResultType calculate_w320( const Triangulation& surface,
                                                const CompWiseScalarMinkValResultType& w300 = CompWiseScalarMinkValResultType(),
                                                const CompWiseVectorMinkValResultType& w310 = CompWiseVectorMinkValResultType());

CompWiseMatrixMinkValResultType calculate_w102(const Triangulation& surface);
CompWiseMatrixMinkValResultType calculate_w202(const Triangulation& surface);

CompWiseTensor4MinkValResultType calculate_w104(const Triangulation& surface);
CompWiseTensor3MinkValResultType calculate_w103(const Triangulation& surface);

CompWiseSphMinkMinkValResultType calculate_sphmink(const Triangulation& surface);

inline Vector get_ref_vec (const int &label, const CompWiseScalarMinkValResultType w_scalar, const CompWiseVectorMinkValResultType w_vector){

    std::map<unsigned int,ScalarMinkValResultType>::const_iterator it1;
    it1 = w_scalar.find (label);
    std::map<unsigned int,VectorMinkValResultType>::const_iterator it2;
    it2 = w_vector.find (label);

    return it2->second.result_/it1->second.result_;
}

#endif // CALCULATE_H_INCLUDED
