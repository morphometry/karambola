#include "print_functions.h"
#include <iostream>

void print_CompWiseVectorMinkValResultType(CompWiseVectorMinkValResultType w){
//	std::cout << "Results for " << name << " :" << std::endl;
	std::map <unsigned int,VectorMinkValResultType>::iterator it;
		for (it = w.begin (); it != w.end (); ++it) {
			int label;
			VectorMinkValResultType w_labeled;
			label = it->first;
			w_labeled = it->second;
        	std::cout << "label " << label << " : " << w_labeled.name_ << " = ( " << w_labeled.result_[0] << " , " << w_labeled.result_[1] << " , " << w_labeled.result_[2] << " )" << std::endl;
	}
	std::cout << std::endl;
}

