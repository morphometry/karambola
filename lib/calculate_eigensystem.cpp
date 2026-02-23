#include "calculate_eigensystem.h"
#include "common.h"
#include "gsleig.h"
#include "Kmath.h"
#include <cassert>

CompWiseEigenSystemMinkValResultType calculate_eigensystem(CompWiseMatrixMinkValResultType& w){

    CompWiseEigenSystemMinkValResultType w_eigsys;

    std::map <unsigned int,MatrixMinkValResultType>::iterator it;

    for (it = w.begin (); it != w.end (); ++it) {
        int label;
        MatrixMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;
        w_eigsys[label].name_ = w_labeled.name_ +"_eigsys";

        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++)
            w_eigsys[label].append_keyword(w_labeled.keywords_.at(i));


        bool calc_eigsys = true;
        for (unsigned int m = 0; m<3; m++){
            for (unsigned int n = 0; n<3; n++){
                if(is_nan(w_labeled.result_(m,n)))
                    calc_eigsys = false;
            }
        }
        if (calc_eigsys == true){
            EigensystemWorkspace ew;
            ew.diagonalize_matrix (w_labeled.result_);
            for (int i = 0; i != 3; ++i){
                w_eigsys[label].result_.eigen_values[i] = ew.eigenvalue(i);
                if(ew.eigenvector(i)[2]<0){
                    w_eigsys[label].result_.eigen_vectors[i][0] = -1*ew.eigenvector(i)[0];
                    w_eigsys[label].result_.eigen_vectors[i][1] = -1*ew.eigenvector(i)[1];
                    w_eigsys[label].result_.eigen_vectors[i][2] = -1*ew.eigenvector(i)[2];
                }
                else{
                    w_eigsys[label].result_.eigen_vectors[i][0] = ew.eigenvector(i)[0];
                    w_eigsys[label].result_.eigen_vectors[i][1] = ew.eigenvector(i)[1];
                    w_eigsys[label].result_.eigen_vectors[i][2] = ew.eigenvector(i)[2];
                }
            }
        }
        if (calc_eigsys == false){
            for (int i = 0; i != 3; ++i){
                w_eigsys[label].result_.eigen_values[i] = NAN;
                w_eigsys[label].result_.eigen_vectors[i][0] = NAN;
                w_eigsys[label].result_.eigen_vectors[i][1] = NAN;
                w_eigsys[label].result_.eigen_vectors[i][2] = NAN;
            }
        }
    }
    return w_eigsys;

}
