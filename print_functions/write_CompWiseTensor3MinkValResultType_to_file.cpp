#include "write_functions.h"
#include "print_explanations.h"
#include <fstream>
#include <filesystem>

void write_CompWiseTensor3MinkValResultType_to_file(const CalcOptions& CO, const CompWiseTensor3MinkValResultType &w){

    if (w.size() == 0) return;


    std::map <unsigned int,Tensor3MinkValResultType>::const_iterator it;
    it = w.begin ();
    std::string filename = CO.outfoldername + "/" + it->second.name_;
    std::string w_name = it->second.name_;

    if(CO.get_compute(w_name) == false && CO.get_force(w_name) == false) return;

    std::ofstream wfile;
    std::filesystem::create_directories(CO.outfoldername);
    wfile.open (filename.c_str());

    int sw = 20;
    wfile.precision(12);

    wfile << "#";
    wfile << std::setw(sw-1) <<"#1 label";
    wfile << std::setw(sw) <<"#2  m(0,0,0)";
    wfile << std::setw(sw) <<"#3  m(0,0,1)";
    wfile << std::setw(sw) <<"#4  m(0,0,2)";
    wfile << std::setw(sw) <<"#5  m(0,1,0)";
    wfile << std::setw(sw) <<"#6  m(0,1,1)";
    wfile << std::setw(sw) <<"#7  m(0,1,2)";
    wfile << std::setw(sw) <<"#8  m(0,2,0)";
    wfile << std::setw(sw) <<"#9  m(0,2,1)";
    wfile << std::setw(sw) <<"#10 m(0,2,2)"; 
    wfile << std::setw(sw) <<"#11 m(1,0,0)";
    wfile << std::setw(sw) <<"#12 m(1,0,1)";
    wfile << std::setw(sw) <<"#13 m(1,0,2)";
    wfile << std::setw(sw) <<"#14 m(1,1,0)";
    wfile << std::setw(sw) <<"#15 m(1,1,1)";
    wfile << std::setw(sw) <<"#16 m(1,1,2)";
    wfile << std::setw(sw) <<"#17 m(1,2,0)";
    wfile << std::setw(sw) <<"#18 m(1,2,1)";
    wfile << std::setw(sw) <<"#19 m(1,2,2)"; 
    wfile << std::setw(sw) <<"#20 m(2,0,0)";
    wfile << std::setw(sw) <<"#21 m(2,0,1)";
    wfile << std::setw(sw) <<"#22 m(2,0,2)";
    wfile << std::setw(sw) <<"#23 m(2,1,0)";
    wfile << std::setw(sw) <<"#24 m(2,1,1)";
    wfile << std::setw(sw) <<"#25 m(2,1,2)";
    wfile << std::setw(sw) <<"#26 m(2,2,0)";
    wfile << std::setw(sw) <<"#27 m(2,2,1)";
    wfile << std::setw(sw) <<"#28 m(2,2,2)"; 
    wfile << std::setw(sw) <<"#29 name"; 
    wfile << std::setw(sw) <<"#30 Keywords"; 
    wfile << std::endl;

    for (it = w.begin (); it != w.end (); ++it) {
        int label;
        Tensor3MinkValResultType w_labeled;
        label = it->first;
        w_labeled = it->second;

        if (label == -300)
            wfile << std::setw(sw) << "ALL";
        else
            wfile << std::setw(sw) << label;
        for (unsigned int m = 0; m<3; m++){
            for (unsigned int n = 0; n<3; n++){
                for (unsigned int p = 0; p<3; p++){
                    if(is_nan(w_labeled.result_(m,n,p)))
                        wfile << std::setw(sw) << "ERROR";
                    else
                        wfile << std::setw(sw) << w_labeled.result_(m,n,p);
                }
            }
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
