#include "calculate_wxxx.h"
#include "calculate.h"
#include <math.h>
#include <iostream>
#include <string>

template <typename T> void set_NAN (T &input);
template <> void set_NAN (double &input){
    input = NAN;
}
template <> void set_NAN (Vector &input){
    Vector test(NAN,NAN,NAN);
    input = test;
}
template <int U> void set_NAN (SymmetricMatrix<U> &input){
    SymmetricMatrix<U> test;
    for (size_t a = 0; a < test.size1(); a++)
    for (size_t b = a; b < test.size2(); b++)
        test(a,b) = NAN;
    input = test;
}
template <> void set_NAN (SymmetricRank4Tensor &input){
    SymmetricRank4Tensor test;
    for (size_t a = 0; a < test.size1(); a++)
    for (size_t b = a; b < test.size2(); b++)
        test(a,b) = NAN;
    input = test;
}
template <> void set_NAN (Rank3Tensor &input){
    Rank3Tensor test;
    for (size_t a = 0; a < 3; a++)
    for (size_t b = 0; b < 3; b++)
    for (size_t c = 0; c < 3; c++)
        test(a,b,c) = NAN;
    input = test;
}
template <> void set_NAN (SphMink &input){
    SphMink test;
    for (size_t a = 0; a < 12; a++){
         test.ql.at(a) = NAN;
         test.wl.at(a) = NAN;
    }
    input = test;
}


template <typename T>
T calculate_w ( const std::string &name,
                const Triangulation& surface,
                const CompWiseScalarMinkValResultType &w_scalar,
                const CompWiseVectorMinkValResultType &w_vector);
//                const CompWiseScalarMinkValResultType &w_scalar = CompWiseScalarMinkValResultType(),
//                const CompWiseVectorMinkValResultType &w_vector = CompWiseVectorMinkValResultType());

template <>
CompWiseScalarMinkValResultType calculate_w (const std::string &name,
                                             const Triangulation& surface,
                                             const CompWiseScalarMinkValResultType &w_scalar,
                                             const CompWiseVectorMinkValResultType &w_vector){

    if(name == "w000")
        return calculate_w000(surface);
    if(name == "w100")
        return calculate_w100(surface);
    if(name == "w200")
        return calculate_w200(surface);
    if(name == "w300")
        return calculate_w300(surface);
    abort ();
}

template <>
CompWiseVectorMinkValResultType calculate_w (const std::string &name,
                                             const Triangulation& surface,
                                             const CompWiseScalarMinkValResultType &w_scalar,
                                             const CompWiseVectorMinkValResultType &w_vector){

    if(name == "w010")
        return calculate_w010(surface);
    if(name == "w110")
        return calculate_w110(surface);
    if(name == "w210")
        return calculate_w210(surface);
    if(name == "w310")
        return calculate_w310(surface);
    abort ();
}

template <>
CompWiseMatrixMinkValResultType calculate_w (const std::string &name,
                                             const Triangulation &surface,
                                             const CompWiseScalarMinkValResultType &w_scalar,
                                             const CompWiseVectorMinkValResultType &w_vector){
    if(name == "w020")
        return calculate_w020(surface, w_scalar, w_vector);
    if(name == "w120")
        return calculate_w120(surface, w_scalar, w_vector);
    if(name == "w220")
        return calculate_w220(surface, w_scalar, w_vector);
    if(name == "w320")
        return calculate_w320(surface, w_scalar, w_vector);
    if(name == "w102")
        return calculate_w102(surface);
    if(name == "w202")
        return calculate_w202(surface);
    abort ();
}

template <>
CompWiseTensor4MinkValResultType calculate_w (const std::string &name,
                                              const Triangulation &surface,
                                              const CompWiseScalarMinkValResultType &w_scalar,
                                              const CompWiseVectorMinkValResultType &w_vector){
    if(name == "w104")
        return calculate_w104(surface);
    abort ();
}

template <>
CompWiseTensor3MinkValResultType calculate_w (const std::string &name,
                                              const Triangulation &surface,
                                              const CompWiseScalarMinkValResultType &w_scalar,
                                              const CompWiseVectorMinkValResultType &w_vector){
    if(name == "w103")
        return calculate_w103(surface);
    abort ();
}

template <>
CompWiseSphMinkMinkValResultType calculate_w (const std::string &name,
                                             const Triangulation &surface,
                                             const CompWiseScalarMinkValResultType &w_scalar,
                                             const CompWiseVectorMinkValResultType &w_vector){
    if(name == "msm")
        return calculate_sphmink(surface);
    abort ();
}

template<typename T>
T calculate_wxxx(   const std::string name,
                    const CalcOptions& CO,
                    const Triangulation& surface,
                    const CompWiseScalarMinkValResultType w_scalar,
                    const CompWiseVectorMinkValResultType w_vector){
//                    const CompWiseScalarMinkValResultType w_scalar = CompWiseScalarMinkValResultType(),
//                    const CompWiseVectorMinkValResultType w_vector = CompWiseVectorMinkValResultType()){
//    CompWiseResultType<T> w;
    T w;
    if(CO.get_compute(name) == false && CO.get_force(name) == false)
        return w;
//    std::cout << "calculate " << name << " ...";
//    std::cout.flush();

    if (CO.reference_origin == false)
        w = calculate_w<T>(name, surface, w_scalar, w_vector); 
    else
        w = calculate_w<T>(name, surface); 


//    add_surface_props;
    for (w.it = w.begin (); w.it != w.end (); ++w.it) {

        w.it->second.name_ = name;

        int test = CO.get_label_closed_status (w.it->first);
        if (test == 0) w.it->second.append_keyword( "closed" );
        if (test == 1) w.it->second.append_keyword ( "shared" );
        if (test == 2) w.it->second.append_keyword ("open" );
        if( CO.reference_origin == true)
            w.it->second.append_keyword(reference_origin_string.first);
        else
            w.it->second.append_keyword(reference_centroid_string.first);
    }

//FIXME for tensors
#if 0
    //look for empty labels
    std::map <int,int>::const_iterator it2;
    for (it2 = CO.labeled_surfaces_closed.begin (); it2 != CO.labeled_surfaces_closed.end (); ++it2) {
        if ( w[it2->first].name_ =="name_not_yet_assigned"){
            w[it2->first].name_ = name;
            for (unsigned int m = 0; m<3; m++){
                for (unsigned int n = 0; n<3; n++){
                    w[it2->first].result_(m,n) = NAN;
                }
            }
            w[it2->first].append_keyword(cant_calc.first);
        }
    }
#endif

    //check if w can be calculated or is forced to be calculated
    for (w.it = w.begin (); w.it != w.end (); ++w.it) {
        if(CO.get_label_closed_status(w.it->first) > CO.get_allowed_to_calc(name)){
            if (CO.get_force(name) == true){
                w.it->second.append_keyword(calc_forced.first);
            }
            else{
                set_NAN(w.it->second.result_);
                w.it->second.append_keyword(cant_calc.first);
            }
        }
    }

//    std::cout << "     done" << std::endl;
    return w;
}

template CompWiseScalarMinkValResultType calculate_wxxx(
                    const std::string name,
                    const CalcOptions& CO,
                    const Triangulation& surface,
                    const CompWiseScalarMinkValResultType w_scalar,
                    const CompWiseVectorMinkValResultType w_vector);

template CompWiseVectorMinkValResultType calculate_wxxx(
                    const std::string name,
                    const CalcOptions& CO,
                    const Triangulation& surface,
                    const CompWiseScalarMinkValResultType w_scalar,
                    const CompWiseVectorMinkValResultType w_vector);

template CompWiseMatrixMinkValResultType calculate_wxxx(
                    const std::string name,
                    const CalcOptions& CO,
                    const Triangulation& surface,
                    const CompWiseScalarMinkValResultType w_scalar,
                    const CompWiseVectorMinkValResultType w_vector);

template CompWiseTensor3MinkValResultType calculate_wxxx(
                    const std::string name,
                    const CalcOptions& CO,
                    const Triangulation& surface,
                    const CompWiseScalarMinkValResultType w_scalar,
                    const CompWiseVectorMinkValResultType w_vector);

template CompWiseTensor4MinkValResultType calculate_wxxx(
                    const std::string name,
                    const CalcOptions& CO,
                    const Triangulation& surface,
                    const CompWiseScalarMinkValResultType w_scalar,
                    const CompWiseVectorMinkValResultType w_vector);

template CompWiseSphMinkMinkValResultType calculate_wxxx(
                    const std::string name,
                    const CalcOptions& CO,
                    const Triangulation& surface,
                    const CompWiseScalarMinkValResultType w_scalar,
                    const CompWiseVectorMinkValResultType w_vector);

