#include "image.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;


/// @brief This function applies the sobel operator to an image and writes the result to a pgm file.
/// @param input 
/// @param output 


void h1(std::string input, std::string output){
    // Read the image
    Image an_image;
    Image output_image;
    // Check if the image can be read
    if (!ReadImage(input, &an_image)) {
        std::cout <<"Can't open file " << input << std::endl;
        return;
    }
    // Allocate space for the output image
    output_image.AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
    // Iterate through the image and each pixel to zero. Preallocate the space for the output image and prepere the output image
    for(int i = 0; i < an_image.num_columns(); i++){
        for(int j = 0; j < an_image.num_rows(); j++){
            output_image.SetPixel(j, i, 0);
        }
    }
    // Iterate through the image and apply the sobel operator
    std::vector<std::vector<int>> sobetX = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    std::vector<std::vector<int>> sobetY = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    // Iterate through the image and apply the sobel operator
    for (int i = 1; i < an_image.num_columns() - 1; i++) {
        for (int j = 1; j < an_image.num_rows() - 1; j++) {
            int sumX = 0;
            int sumY = 0;
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    // Apply the sobel operator
                    int x = i + k - 1;
                    int y = j + l - 1;
                    sumX += an_image.GetPixel(y, x) * sobetX[k][l];
                    sumY += an_image.GetPixel(y, x) * sobetY[k][l];
                }
            }
            // Calculate the magnitude of the gradient
            int sum = std::sqrt(sumX * sumX + sumY * sumY);
            // Set the pixel to the magnitude of the gradient but ig greater than 255 set it to 255
            if (sum > 255) {
                sum = 255;
            }
            output_image.SetPixel(j, i, sum);
        }
    }
    // Set the number of gray levels in the output image and write it to a file.
    output_image.SetNumberGrayLevels(255);
    // Write the image to the output file
    WriteImage(output, output_image);
}

int main(int argc, char **argv){
    // Check if the number of arguments is correct
    if(argc != 3){
        std::cout<<"Usage: "<<argv[0]<<" <input_image.pgm> <output_image.pgm>\n";
        return 0;
    }
    std::string input(argv[1]);
    std::string output(argv[2]);
    h1(input, output);
    return 0;
}
