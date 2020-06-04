#include "print_functions.h"
#include <iostream>

void print_CompWiseMatrixMinkValResultType(CompWiseMatrixMinkValResultType w){
//	std::cout << "Results for " << name << " :" << std::endl;
	std::map <unsigned int,MatrixMinkValResultType>::iterator it;
		for (it = w.begin (); it != w.end (); ++it) {
			int label;
			MatrixMinkValResultType w_labeled;
			label = it->first;
			w_labeled = it->second;

		int sw = 12;
		std::cout.precision(5);	
		std::cout << std::setw(6) << "label " << std::setw(4) << label << std::setw(3) << " : " << std::setw(4) << w_labeled.name_ << std::setw(4) << " =  " ;
		std::cout << std::setw(sw) << w_labeled.result_(0,0) << std::setw(sw) << w_labeled.result_(0,1) << std::setw(sw) << w_labeled.result_(0,2);
		std::cout << std::endl;
		std::cout << std::setw(21) << " ";
		std::cout << std::setw(sw) << w_labeled.result_(1,0) << std::setw(sw) << w_labeled.result_(1,1) << std::setw(sw) << w_labeled.result_(1,2);
		std::cout << std::endl;
		std::cout << std::setw(21) << " ";
		std::cout << std::setw(sw) << w_labeled.result_(2,0) << std::setw(sw) << w_labeled.result_(2,1) << std::setw(sw) << w_labeled.result_(2,2);
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

