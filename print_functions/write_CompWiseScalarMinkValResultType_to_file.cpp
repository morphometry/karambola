#include "write_functions.h"

void write_CompWiseScalarMinkValResultType_to_file( const CalcOptions& CO,
                                                    const CompWiseScalarMinkValResultType &w000,
                                                    const CompWiseScalarMinkValResultType &w100,
                                                    const CompWiseScalarMinkValResultType &w200,
                                                    const CompWiseScalarMinkValResultType &w300,
                                                    bool append){
    std::ofstream wfile;

    mkdir(CO.outfoldername.c_str(),0755);    

    std::string filename = CO.outfoldername + "/w000_w100_w200_w300";

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
        wfile << std::setw(sw) <<"#2 w"; 
        wfile << std::setw(sw) <<"#3 name"; 
        wfile << std::setw(sw) <<"#4 Keywords"; 
        wfile << std::endl;
    }

    std::map <unsigned int,ScalarMinkValResultType>::const_iterator it;
    for (it = w000.begin (); it != w000.end (); ++it) {
        int label;
        ScalarMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;

        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else    
            wfile << std::setw(sw) << label;

        if(is_nan(w_labeled.result_))
            wfile << std::setw(sw) << "ERROR";
        else    
            wfile << std::setw(sw) << w_labeled.result_;

        wfile << std::setw(sw) << w_labeled.name_;

        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    for (it = w100.begin (); it != w100.end (); ++it) {
        int label;
        ScalarMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;

        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else    
            wfile << std::setw(sw) << label;

        if(is_nan(w_labeled.result_))
            wfile << std::setw(sw) << "ERROR";
        else    
            wfile << std::setw(sw) << w_labeled.result_;

        wfile << std::setw(sw) << w_labeled.name_;

        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    for (it = w200.begin (); it != w200.end (); ++it) {
        int label;
        ScalarMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;

        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else    
            wfile << std::setw(sw) << label;

        if(is_nan(w_labeled.result_))
            wfile << std::setw(sw) << "ERROR";
        else    
            wfile << std::setw(sw) << w_labeled.result_;

        wfile << std::setw(sw) << w_labeled.name_;

        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    for (it = w300.begin (); it != w300.end (); ++it) {
        int label;
        ScalarMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;

        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else    
            wfile << std::setw(sw) << label;

        if(is_nan(w_labeled.result_))
            wfile << std::setw(sw) << "ERROR";
        else    
            wfile << std::setw(sw) << w_labeled.result_;

        wfile << std::setw(sw) << w_labeled.name_;

        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

    wfile.close();
}
