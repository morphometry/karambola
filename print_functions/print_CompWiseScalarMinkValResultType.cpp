#include "print_functions.h"

void print_CompWiseScalarMinkValResultType(CompWiseScalarMinkValResultType w){
//	std::cout << "Results for " << name << " :" << std::endl;
	std::map <unsigned int,ScalarMinkValResultType>::iterator it;
		for (it = w.begin (); it != w.end (); ++it) {
			int label;
			ScalarMinkValResultType w_labeled;
			label = it->first;
			w_labeled = it->second;
        	std::cout << "label " << label << " : " << w_labeled.name_ << " = " << w_labeled.result_ << std::endl;
	}
	std::cout << std::endl;
}

