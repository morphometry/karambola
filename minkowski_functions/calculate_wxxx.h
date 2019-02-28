#ifndef CALCULATE_WXXX
#define CALCULATE_WXXX

#include "calculate.h"
#include <math.h>
#include <iostream>
#include <string>

template <typename T> void set_NAN (T &input);
template <> void set_NAN (double &input);
template <> void set_NAN (Vector &input);

template <int U> void set_NAN (SymmetricMatrix<U> &input);
template <> void set_NAN (SymmetricRank4Tensor &input);
template <> void set_NAN (Rank3Tensor &input);
template <> void set_NAN (SphMink &input);


template <typename T>
T calculate_w ( const std::string &name,
                const Triangulation& surface,
                const CompWiseScalarMinkValResultType &w_scalar = CompWiseScalarMinkValResultType(),
                const CompWiseVectorMinkValResultType &w_vector = CompWiseVectorMinkValResultType());

template <>
CompWiseScalarMinkValResultType calculate_w (const std::string &name,
                                             const Triangulation& surface,
                                             const CompWiseScalarMinkValResultType &w_scalar,
                                             const CompWiseVectorMinkValResultType &w_vector);

template <>
CompWiseVectorMinkValResultType calculate_w (const std::string &name,
                                             const Triangulation& surface,
                                             const CompWiseScalarMinkValResultType &w_scalar,
                                             const CompWiseVectorMinkValResultType &w_vector);

template <>
CompWiseMatrixMinkValResultType calculate_w (const std::string &name,
                                             const Triangulation &surface,
                                             const CompWiseScalarMinkValResultType &w_scalar,
                                             const CompWiseVectorMinkValResultType &w_vector);

template <>
CompWiseTensor4MinkValResultType calculate_w (const std::string &name,
                                              const Triangulation &surface,
                                              const CompWiseScalarMinkValResultType &w_scalar,
                                              const CompWiseVectorMinkValResultType &w_vector);


template <>
CompWiseTensor3MinkValResultType calculate_w (const std::string &name,
                                              const Triangulation &surface,
                                              const CompWiseScalarMinkValResultType &w_scalar,
                                              const CompWiseVectorMinkValResultType &w_vector);

template <>
CompWiseSphMinkMinkValResultType calculate_w (const std::string &name,
                                              const Triangulation &surface,
                                              const CompWiseScalarMinkValResultType &w_scalar,
                                              const CompWiseVectorMinkValResultType &w_vector);

template<typename T>
T calculate_wxxx(   const std::string name,
                    const CalcOptions& CO,
                    const Triangulation& surface,
                    const CompWiseScalarMinkValResultType w_scalar = CompWiseScalarMinkValResultType(),
                    const CompWiseVectorMinkValResultType w_vector = CompWiseVectorMinkValResultType());


#endif // CALCULATE_XXX

