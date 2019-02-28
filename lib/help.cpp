#include "help.h"

#include <iostream>


void print_short_help() {
    std::cout << "To start karambola, please use: \n"
                 "      ./karambola -i input-file.poly {--labels | --no-labels} [-o outputdirname][more options...]\n"
                 "use ./karambola --help for more information\n";
}
void print_short_help_bilimbi() {
    std::cout << "use ./bilimbi --help for more information\n";
}

void print_help(){
	std::cout << std::endl;
	std::cout << "To start karambola, use the following syntax:" << std::endl;
    std::cout << "./karambola -i input-file.poly {--labels | --nolabels} [more options...]" << std::endl;
    std::cout << std::endl;
    std::cout << "You have to use one of the the label flags:" << std::endl;
	std::cout << "--nolabels   to indicate that you want to ignore the labels in the input file, if there are any" << std::endl;
    std::cout << "--labels     to indicate that you want to use the labels in the input file" << std::endl;
    std::cout << "(an error occurs if the file has no labels and the -labels option is set)" << std::endl;
    std::cout << std::endl;
    std::cout << "Not all Minkowski Functionals can be calculated for any given input file.  If calculating the MF for a file is not possible, Karambola will indicate that.  If, however, you want to calculate the MF by stubbornly applying the formulae nevertheless, you can use the --force options:\n";
    std::cout << "--force_w000        --force_w200" << std::endl;
    std::cout << "--force_w300        --force_w010" << std::endl;
    std::cout << "--force_w210        --force_w310" << std::endl;
    std::cout << "--force_w010        --force_w210" << std::endl;
    std::cout << "--force_w310" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "if you add the option --reference_centroid , the tensors are calculated in reference to the minkowski-vectors to the and not in reference to the origin" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "you can specify another output directory via -o" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
}


void print_help_bilimbi(){
	std::cout << std::endl;
	std::cout << "To start bilimbi, use the following syntax:" << std::endl;
    std::cout << "./bilimbi -i input-file.poly [more options...]" << std::endl;
    std::cout << std::endl;
    std::cout << "" << std::endl;
    std::cout << "if you add the option --reference_centroid , the tensors are calculated in reference to the minkowski-vectors to the and not in reference to the origin" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "you can specify another output directory via -o" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
}

