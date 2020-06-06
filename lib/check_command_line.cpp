#include "check_command_line.h"

#include "CalcOptions.h"
#include "help.h"


void check_command_line(CalcOptions *CO, int argc, char *argv[]){

    for (int i = 1; i < argc; i++){
        std::string argvi=argv[i];
        if (argvi=="-h" || argvi=="--help")
        {
            print_help();
            exit(1);
        }
    }

    if (argc < 3 ){
        print_short_help();
        exit(-1);
    }

    CO->infilename = "";

    bool surflabelsset=false;
    bool computeset=false;
    CO->reference_origin =true; // default

    for (int i = 1; i < argc; i++){
        std::string argvi=argv[i];
        if (argvi=="--labels"){
            surflabelsset=true;
            CO->labels_set = true;
        }
        else if (argvi == "--nolabels" || argvi == "--no-labels"){
            surflabelsset=true;
            CO->labels_set = false;
        }
        else if (argvi=="-i" || argvi=="--input")
        {
            if (i+1<argc && CO->infilename.size() == 0)
                CO->infilename=argv[++i];
            else {
                print_short_help(); 
                exit(-1);
            }
        }
        else if (argvi=="-o" || argvi=="--output")
        {
            if (i+1<argc)
                CO->outfoldername=argv[++i];
            else {
                print_short_help(); 
                exit(-1);
            }
        }
        //check reference_point for minkowski functionals
        else if(argvi == "--reference_centroid" || argvi == "--reference-centroid")
            CO->reference_origin =false;
        //check if some evaluations want to be forced
        else if(argvi=="--compute"){
            computeset=true;
            CO->set_compute(argv[++i], true);
        }
        else if(argvi=="--force")
            CO->set_force(argv[++i], true);
        else if(argvi[0]!='-')
        {
            // is the filename
            if (CO->infilename.size()>0)
            {
                // already read infilename = more than 1 was given
                print_short_help();
                exit(-1);
            }
            CO->infilename=argvi;
        }
        else
        {
            // is an unknown argument
            print_short_help();
            exit(-1);
        }
    }

    if (!surflabelsset || CO->infilename.size()==0){
        // no infilename was given
        print_short_help();
        exit (-1);
    }

    if (!computeset){
        // no explicit compute was given
        CO->set_compute("w000", true);
        CO->set_compute("w100", true);
        CO->set_compute("w200", true);
        CO->set_compute("w300", true);
        CO->set_compute("w010", true);
        CO->set_compute("w110", true);
        CO->set_compute("w210", true);
        CO->set_compute("w310", true);
        CO->set_compute("w020", true);
        CO->set_compute("w120", true);
        CO->set_compute("w220", true);
        CO->set_compute("w320", true);
        CO->set_compute("w102", true);
        CO->set_compute("w202", true);
        CO->set_compute("msm", true);
    }

    if (CO->outfoldername.size()==0){
        if (CO->infilename.size()>5 && CO->infilename.substr(CO->infilename.size()-5)==".poly")
            CO->outfoldername=CO->infilename.substr(0,CO->infilename.size()-5) + "_mink_val";
        else {
            std::cout << "no outfoldername can be defined" <<std::endl;
            exit(-1);
        }
    }
}
