#include "image.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;

void h2(const string &input, int threshold,  const string &output) {
    Image an_image;
    ReadImage(input, &an_image); 
    for(int i = 0; i < an_image.num_columns(); i++){
        for(int j = 0; j < an_image.num_rows(); j++){
            std::cout<<an_image.GetPixel(j, i)<<std::endl;
            if(an_image.GetPixel(i, j) > threshold){
                an_image.SetPixel(i, j, 255);
            } else {
                an_image.SetPixel(i, j, 0);
            }
        }
    }
    WriteImage(output, an_image);
}

int main(int argc, char **argv) {

    if (argc != 4) {
        std::cout <<
            "Usage: " << argv[0] << " <input_image> <threshold> <output_image>" << std::endl;
        return 0;
    }

    std::string input_image(argv[1]);
    int threshold = std::stoi(argv[2]);
    std::string output_image(argv[3]);

    h2(input_image, threshold, output_image);
    return 0;
}