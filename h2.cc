#include "image.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;


/// @brief H2 takes in a edge pgm file and a threshold and writes a binary pgm file. If the pixel value is greater than the threshold it is set to 255 otherwise it is set to 0.
/// @param input 
/// @param threshold 
/// @param output 


void h2(const string &input, int threshold,  const string &output) {
    // Read the image
    Image an_image;
    ReadImage(input, &an_image); 
    // Iterate through the image and set the pixel to 255 if it is greater than the threshold
    for(int i = 0; i < an_image.num_columns(); i++){
        for(int j = 0; j < an_image.num_rows(); j++){
            if(an_image.GetPixel(j, i) > threshold){
                an_image.SetPixel(j, i, 255);
            } else {
                an_image.SetPixel(j, i, 0);
            }
        }
    }
    // Write the image to the output file
    WriteImage(output, an_image);
}

int main(int argc, char **argv) {
    // Check if the number of arguments is correct
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