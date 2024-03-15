#include "image.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
using namespace ComputerVisionProjects;

void h3(std::string input, std::string outputBinary, std::string utput) {
    Image an_image;
    Image output;

    if (!ReadImage(input, &an_image)) {
        std::cout << "Can't open file " << input << std::endl;
        return;
    }

    std::cout << "Hello" << std::endl;

    int maxRho = std::sqrt(an_image.num_columns() * an_image.num_columns() + an_image.num_rows() * an_image.num_rows());
    output.AllocateSpaceAndSetSize(180 + 1, maxRho); 
    std::cout << "Hello2" << std::endl;
    for(int i = 0; i < an_image.num_rows(); i++){
        for(int j = 0; j < an_image.num_columns(); j++){
            if(an_image.GetPixel(i, j) > 0){
                for(int theta = 0; theta <= 180; theta++){
                    int rho = std::round(j * std::cos(theta * M_PI / 180) + i * std::sin(theta * M_PI / 180));
                    if( rho < 0 || rho > maxRho) continue;
                    output.SetPixel(theta, rho , output.GetPixel(theta, rho) + 1);
                }
            }
        }
    }
    for(int i = 0; i < output.num_columns(); i++){
        for(int j = 0; j < output.num_rows(); j++){
            if(output.GetPixel(j, i) >255){
                output.SetPixel(j, i, 255);
            }
        }
    }
    output.SetNumberGrayLevels(255);
    WriteImage(outputBinary, output);
}

int main(int argc, char **argv){
    if (argc!=4) {
        std::cout << "Usage: " << argv[0] << " <input_image.pgm> <OutputBinrary.pgm> <Output.pgm>" << std::endl;
        return 0;
    }
    std::string input(argv[1]);
    std::string OutputBinrary(argv[2]);
    std::string Output(argv[3]);
    h3(input, OutputBinrary, Output);
    return 0;
}