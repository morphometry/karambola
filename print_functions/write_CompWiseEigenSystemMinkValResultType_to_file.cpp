#include "write_functions.h"
#include "print_explanations.h"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

void write_CompWiseEigenSystemMinkValResultType_to_file(const CalcOptions& CO,
                                                        const CompWiseEigenSystemMinkValResultType &w_eigsys,
                                                        bool append){

    if (w_eigsys.size() == 0) return;


    std::map <unsigned int,EigenSystemMinkValResultType>::const_iterator it;
    it = w_eigsys.begin ();
    std::string filename = CO.outfoldername + "/" + it->second.name_;
    std::string w_eigsys_name = it->second.name_;
    std::string w_name = w_eigsys_name.substr(0,4);

    if(CO.get_compute(w_name) == false && CO.get_force(w_name) == false) return;


    std::ofstream wfile;
    mkdir(CO.outfoldername.c_str(),0755);
    if (append == false) wfile.open (filename.c_str());
    else                 wfile.open (filename.c_str() , std::ios::out | std::ios::app);

    int sw = 20;
    std::cout.precision(12);    
    wfile.precision(12);    

    if (append == false){
        print_explanations(wfile, CO.infilename);
        wfile << "#" << std::endl;
        wfile << "#";
        wfile << std::setw(sw-1) <<"#1 label";
        wfile << std::setw(sw) <<"#2 EVal1";
        wfile << std::setw(sw) <<"#3 EVec1[x]";
        wfile << std::setw(sw) <<"#4 EVec1[y]";
        wfile << std::setw(sw) <<"#5 EVec1[z]";
        wfile << std::setw(sw) <<"#6 EVal2";
        wfile << std::setw(sw) <<"#7 EVec2[x]";
        wfile << std::setw(sw) <<"#8 EVec2[y]";
        wfile << std::setw(sw) <<"#9 EVec2[z]";
        wfile << std::setw(sw) <<"#10 EVal3";
        wfile << std::setw(sw) <<"#11 EVec3[x]";
        wfile << std::setw(sw) <<"#12 EVec3[y]";
        wfile << std::setw(sw) <<"#13 EVec3[z]";
        wfile << std::setw(sw) <<"#14 name";
        wfile << std::setw(sw) <<"#15 Keywords";
        wfile << std::endl;
    }

    for (it = w_eigsys.begin (); it != w_eigsys.end (); ++it) {
        int label;
        EigenSystemMinkValResultType w_eigsys_labeled;
        label = it->first;
        w_eigsys_labeled = it->second;
        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else
            wfile << std::setw(sw) << label;

        for (unsigned int m = 0; m<3; m++){
            wfile << std::setw(sw) << w_eigsys_labeled.result_.eigen_values[m];
            for (unsigned int n = 0; n<3; n++){
                if(is_nan(w_eigsys_labeled.result_.eigen_vectors[m][n]))
                    wfile << std::setw(sw) << "ERROR";
                else
                    wfile << std::setw(sw) << w_eigsys_labeled.result_.eigen_vectors[m][n];



            }
        }
        std::string name = w_eigsys_labeled.name_;
        wfile << std::setw(sw) << name;
        for (unsigned int i = 0; i < w_eigsys_labeled.keywords_.size(); i++)
            wfile << std::setw(sw) << w_eigsys_labeled.keywords_.at(i) << " ";

        wfile << std::endl;
    }

    wfile.close();
}
