#include "write_functions.h"
#include "print_explanations.h"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

void write_CompWiseTensor4MinkValResultType_to_file(const CalcOptions& CO, const CompWiseTensor4MinkValResultType &w){

    if (w.size() == 0) return;



    CompWiseTensor4MinkValResultType::const_iterator it;
    it = w.begin ();
    std::string filename = CO.outfoldername + "/" + it->second.name_ + "_eigval";
    std::string w_name = it->second.name_;

    if(CO.get_compute(w_name) == false && CO.get_force(w_name) == false) return;

    std::ofstream wfile;
    mkdir(CO.outfoldername.c_str(),0755);    
    wfile.open (filename.c_str());

    int sw = 20;
    wfile.precision(12);

    wfile << "#";
    wfile << std::setw(sw-1) <<"#1 label";
    wfile << std::setw(sw) <<"#2 ev0";
    wfile << std::setw(sw) <<"#3 ev1";
    wfile << std::setw(sw) <<"#4 ev2";
    wfile << std::setw(sw) <<"#5 ev3";
    wfile << std::setw(sw) <<"#6 ev4";
    wfile << std::setw(sw) <<"#7 ev5";
    wfile << std::setw(sw) <<"#8 name"; 
    wfile << std::setw(sw) <<"#9 keywords"; 
    wfile << std::endl;

    for (it = w.begin (); it != w.end (); ++it) {
        int label;
        Tensor4MinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;

        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else
            wfile << std::setw(sw) << label;

        std::vector <double> ev = eigenvalues (w_labeled.result_);


        for (unsigned int m = 0; m < ev.size (); m++) {
            if (is_nan (ev.at (m)))
                wfile << std::setw(sw) << "ERROR";
            else
                wfile << std::setw(sw) << ev.at (m);
        }

        //print keywords
        wfile << std::setw(sw) << w_labeled.name_;
        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    print_explanations(wfile, CO.infilename);
    wfile.close();

}
