#include "write_functions.h"
#include "print_explanations.h"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

void write_CompWiseVectorMinkValResultType_to_file( const CalcOptions& CO,
                                                    const CompWiseVectorMinkValResultType &w010,
                                                    const CompWiseVectorMinkValResultType &w110,
                                                    const CompWiseVectorMinkValResultType &w210,
                                                    const CompWiseVectorMinkValResultType &w310,
                                                    bool append){
    std::ofstream wfile;

    mkdir(CO.outfoldername.c_str(),0755);    


    std::string filename = CO.outfoldername + "/w010_w110_w210_w310";

    if (append == false) wfile.open (filename.c_str());
    else                 wfile.open (filename.c_str() , std::ios::out | std::ios::app);

    int sw = 20;
    wfile.precision(12);

    if (append == false){
        print_explanations(wfile, CO.infilename);
        wfile << "#" << std::endl;
        wfile << "#";
        wfile << std::setw(sw-1) <<"#1 label";
        wfile << std::setw(sw) <<"#2 v[x]";
        wfile << std::setw(sw) <<"#3 v[y]";
        wfile << std::setw(sw) <<"#4 v[z]"; 
        wfile << std::setw(sw) <<"#5 name"; 
        wfile << std::setw(sw) <<"#6 Keywords"; 
        wfile << std::endl;
    }

    std::map <unsigned int,VectorMinkValResultType>::const_iterator it;
    for (it = w010.begin (); it != w010.end (); ++it) {
        int label;
        VectorMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;
        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else
            wfile << std::setw(sw) << label;
        for (unsigned int i = 0; i<3; i++){
            if(is_nan(w_labeled.result_[i]))
                wfile << std::setw(sw) << "ERROR";
            else
                wfile << std::setw(sw) << w_labeled.result_[i];
        }
        wfile << std::setw(sw) << w_labeled.name_;
        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    for (it = w110.begin (); it != w110.end (); ++it) {
        int label;
        VectorMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;
        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else
            wfile << std::setw(sw) << label;
        for (unsigned int i = 0; i<3; i++){
            if(is_nan(w_labeled.result_[i]))
                wfile << std::setw(sw) << "ERROR";
            else
                wfile << std::setw(sw) << w_labeled.result_[i];
        }
        wfile << std::setw(sw) << w_labeled.name_;
        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    for (it = w210.begin (); it != w210.end (); ++it) {
        int label;
        VectorMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;
        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else
            wfile << std::setw(sw) << label;
        for (unsigned int i = 0; i<3; i++){
            if(is_nan(w_labeled.result_[i]))
                wfile << std::setw(sw) << "ERROR";
            else
                wfile << std::setw(sw) << w_labeled.result_[i];
        }
        wfile << std::setw(sw) << w_labeled.name_;
        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    for (it = w310.begin (); it != w310.end (); ++it) {
        int label;
        VectorMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;
        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else
            wfile << std::setw(sw) << label;
        for (unsigned int i = 0; i<3; i++){
            if(is_nan(w_labeled.result_[i]))
                wfile << std::setw(sw) << "ERROR";
            else
                wfile << std::setw(sw) << w_labeled.result_[i];
        }
        wfile << std::setw(sw) << w_labeled.name_;

        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    wfile.close();
}
