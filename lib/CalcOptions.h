#ifndef CALCOPTIONS_H
#define CALCOPTIONS_H

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <cstdlib>

extern const std::pair<std::string, std::string> calc_forced;
extern const std::pair<std::string, std::string> cant_calc;
extern const std::pair<std::string, std::string> reference_origin_string;
extern const std::pair<std::string, std::string> reference_centroid_string;

static std::string computable_w[17] = { "w000", "w100", "w200", "w300", "w010", "w110", "w210", "w310", "w020", "w120", "w220", "w320", "w102", "w202", "w103","w104", "msm"};


class CalcOptions{
    public:
        CalcOptions(){  //infilename = "NA";
                        //outfoldername = ".";
                        reference_origin = true;
                        for (int i = 0; i<17; i++ )
                            compute[computable_w[i]] = false;
                        for (int i = 0; i<17; i++ )
                            force[computable_w[i]] = false;
                        allowed_to_calc["w000"] = 0; //only for closed
                        allowed_to_calc["w100"] = 2; //for all
                        allowed_to_calc["w200"] = 1; //for closed and shared
                        allowed_to_calc["w300"] = 1;
                        allowed_to_calc["w010"] = 0;
                        allowed_to_calc["w110"] = 2;
                        allowed_to_calc["w210"] = 1;
                        allowed_to_calc["w310"] = 1;
                        allowed_to_calc["w020"] = 0;
                        allowed_to_calc["w120"] = 2;
                        allowed_to_calc["w220"] = 1;
                        allowed_to_calc["w320"] = 1;
                        allowed_to_calc["w102"] = 2;
                        allowed_to_calc["w104"] = 2;
                        allowed_to_calc["w202"] = 1;
                        allowed_to_calc["w103"] = 2;
                        allowed_to_calc["w104"] = 2;
                        allowed_to_calc["msm"] = 2;
            }

        bool reference_origin;        
        std::string infilename;
        std::string outfoldername;


        void set_compute (std::string w, bool status)
        {
            if (w_computable(w) == true)
                compute[w] = status;
            else{
                std::cerr << "I don't know " << w << "\n";
                std::abort ();
            }
        }

        void set_force (std::string w, bool status)
        {
            if (w_computable(w) == true)
                force[w] = status;
            else{
                std::cerr << "I don't know " << w << "\n";
                std::abort ();
            }
        }

        bool get_compute (std::string w) const
        {
            std::map<std::string,bool>::const_iterator it;
            it = compute.find (w);
            if (it == compute.end ())
            {
                std::cerr << "compute status " << w << " requested, which I have no clue about. This is a bug in Karambola.\n";
                std::abort ();
            }
            return it->second;
        }

        bool get_force (std::string w) const
        {
            std::map<std::string,bool>::const_iterator it;
            it = force.find (w);
            if (it == force.end ())
            {
                std::cerr << "force status " << w << " requested, which I have no clue about. This is a bug in Karambola.\n";
                std::abort ();
            }
            return it->second;
        }

        int get_allowed_to_calc (std::string w) const
        {
            std::map<std::string,int>::const_iterator it;
            it = allowed_to_calc.find (w);
            if (it == allowed_to_calc.end ())
            {
                std::cerr << "allowed_to_calc status " << w << " requested, which I have no clue about. This is a bug in Karambola.\n";
                std::abort ();
            }
            return it->second;
        }


        //only for polys
        bool labels_set;

        int get_label_closed_status (int label) const
        {
            std::map<int,int>::const_iterator it;
            it = labeled_surfaces_closed.find (label);
            if (it == labeled_surfaces_closed.end ())
            {
                std::cerr << "label " << label << " requested, which I have no clue about. This is a bug in Karambola.\n";
                std::abort ();
            }
            return it->second;
        }

        void create_label (int label, int status)
        {
            labeled_surfaces_closed[label] = status;
        }

        std::map<int,int> labeled_surfaces_closed;    

        //only for bin
        bool periodic;
        bool voxel_hits_bondary;

    private:
        bool w_computable (std::string w){
//            for (int i = 0; i<(sizeof(computable_w)/sizeof(computable_w[0])); i++ ){
            for (int i = 0; i<17; i++ ){
                if (w == computable_w[i])
                    return true;
            }
            return false;
        }
        std::map<std::string, bool> compute;
        std::map<std::string, bool> force;
        std::map<std::string, int> allowed_to_calc;
        CalcOptions (const CalcOptions &);
};





#if 0
inline
std::ostream &operator<< (std::ostream &os, const CalcOptions &rhs)
{
    return os << "CalcOptions object, number of labels = " << rhs.labeled_surfaces_closed.size () << "\n";
}
#endif
#endif
