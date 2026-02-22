#include "write_functions.h"
#include "print_explanations.h"
#include <fstream>
#include <filesystem>

void write_CompWiseMatrixMinkValResultType_to_file(const CalcOptions& CO, const CompWiseMatrixMinkValResultType &w, bool append){

    if (w.size() == 0) return;


    std::map <unsigned int,MatrixMinkValResultType>::const_iterator it;
    it = w.begin ();
    std::string filename = CO.outfoldername + "/" + it->second.name_;
    std::string w_name = it->second.name_;

    if(CO.get_compute(w_name) == false && CO.get_force(w_name) == false) return;

    std::ofstream wfile;
    std::filesystem::create_directories(CO.outfoldername);
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
        wfile << std::setw(sw) <<"#2 m(0,0)";
        wfile << std::setw(sw) <<"#3 m(0,1)";
        wfile << std::setw(sw) <<"#4 m(0,2)";
        wfile << std::setw(sw) <<"#5 m(1,0)";
        wfile << std::setw(sw) <<"#6 m(1,1)";
        wfile << std::setw(sw) <<"#7 m(1,2)";
        wfile << std::setw(sw) <<"#8 m(2,0)";
        wfile << std::setw(sw) <<"#9 m(2,1)";
        wfile << std::setw(sw) <<"#10 m(2,2)"; 
        wfile << std::setw(sw) <<"#11 name"; 
        wfile << std::setw(sw) <<"#12 Keywords"; 
        wfile << std::endl;
    }

    for (it = w.begin (); it != w.end (); ++it) {
        int label;
        MatrixMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;

        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else
            wfile << std::setw(sw) << label;
        for (unsigned int m = 0; m<3; m++){
            for (unsigned int n = 0; n<3; n++){
                if(is_nan(w_labeled.result_(m,n)))
                    wfile << std::setw(sw) << "ERROR";
                else
                    wfile << std::setw(sw) << w_labeled.result_(m,n);
            }
        }

        //print keywords
        wfile << std::setw(sw) << w_labeled.name_;
        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    wfile.close();

}
