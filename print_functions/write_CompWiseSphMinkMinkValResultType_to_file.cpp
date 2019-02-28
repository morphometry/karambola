#include "write_functions.h"

void write_CompWiseSphMinkMinkValResultType_to_file( const CalcOptions& CO,
                                                    const CompWiseSphMinkMinkValResultType &sphmink,
                                                    bool append){
    std::ofstream wfile;

    mkdir(CO.outfoldername.c_str(),0755);    


    std::string filename = CO.outfoldername + "/msm_ql";

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
        wfile << std::setw(sw) <<"#2 q(0)";
        wfile << std::setw(sw) <<"#3 w(0)";
        wfile << std::setw(sw) <<"#4 q(1)"; 
        wfile << std::setw(sw) <<"#5 w(1)"; 
        wfile << std::setw(sw) <<"#6 q(2)"; 
        wfile << std::setw(sw) <<"#7 w(2)"; 
        wfile << std::setw(sw) <<"#8 q(3)"; 
        wfile << std::setw(sw) <<"#9 w(3)"; 
        wfile << std::setw(sw) <<"#10 q(4)"; 
        wfile << std::setw(sw) <<"#11 w(4)"; 
        wfile << std::setw(sw) <<"#12 q(5)"; 
        wfile << std::setw(sw) <<"#13 w(5)"; 
        wfile << std::setw(sw) <<"#14 q(6)"; 
        wfile << std::setw(sw) <<"#15 w(6)"; 
        wfile << std::setw(sw) <<"#16 q(7)"; 
        wfile << std::setw(sw) <<"#17 w(7)"; 
        wfile << std::setw(sw) <<"#18 q(8)"; 
        wfile << std::setw(sw) <<"#19 w(8)"; 
        wfile << std::setw(sw) <<"#20 Keywords"; 
        wfile << std::endl;
    }

    std::map <unsigned int,SphMinkMinkValResultType>::const_iterator it;
    for (it = sphmink.begin (); it != sphmink.end (); ++it) {
        int label;
        SphMinkMinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;
        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else
            wfile << std::setw(sw) << label;
        for (unsigned int i = 0; i<9; i++){
            if(is_nan(w_labeled.result_.ql.at(i)))
                wfile << std::setw(sw) << "ERROR";
            else
                wfile << std::setw(sw) << w_labeled.result_.ql.at(i);
            if(is_nan(w_labeled.result_.wl.at(i)))
                wfile << std::setw(sw) << "ERROR";
            else
                wfile << std::setw(sw) << w_labeled.result_.wl.at(i);
        }
        wfile << std::setw(sw) << w_labeled.name_;
        for (unsigned int i = 0; i < w_labeled.keywords_.size(); i++){
            wfile << std::setw(sw) << w_labeled.keywords_.at(i) << " ";
        }
        wfile << std::endl;
    }

 
    wfile.close();
}
