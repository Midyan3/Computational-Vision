#include "image.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>  // Include fstream for file operations
using namespace ComputerVisionProjects;

void h3(std::string input, std::string outputBinary, std::string outputText) {
    Image an_image;
    Image output;

    if (!ReadImage(input, &an_image)) {
        std::cout << "Can't open file " << input << std::endl;
        return;
    }

    std::cout << "Hello" << std::endl;
    int maxRho = std::sqrt(an_image.num_columns() * an_image.num_columns() + an_image.num_rows() * an_image.num_rows());
    std::vector<std::vector<int>> Accumulator(180 + 1, std::vector<int>(maxRho, 0));
    output.AllocateSpaceAndSetSize(180 + 1, maxRho); 
    std::cout << "Hello2" << std::endl;
    for(int i = 0; i < an_image.num_rows(); i++){
        for(int j = 0; j < an_image.num_columns(); j++){
            if(an_image.GetPixel(i, j) > 0){
                for(int theta = 0; theta <= 180; theta++){
                    int rho = std::round(j * std::cos(theta * M_PI / 180) + i * std::sin(theta * M_PI / 180));
                    if( rho < 0 || rho > maxRho) continue;
                    output.SetPixel(theta, rho , output.GetPixel(theta, rho) + 1);
                    Accumulator[theta][rho] += 1;

                }
            }
        }
    }

      std::ofstream outFile(outputText);
    if (!outFile) {
        std::cout << "Can't open file " << outputText << std::endl;
        return;
    }

    for(int theta = 0; theta < Accumulator.size(); theta++){
        for(int rho = 0; rho < Accumulator[theta].size(); rho++){
            if (Accumulator[theta][rho] > 0) {
                outFile << theta << " " << rho << " " << Accumulator[theta][rho] << "\n";
            }
        }
    }

    outFile.close();

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





/*

#include "image.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
using namespace ComputerVisionProjects;
#include <fstream>  // Include fstream for file operations

void h3(std::string input, std::string outputBinary, std::string outputText) {
    Image an_image;
    Image output;

    if (!ReadImage(input, &an_image)) {
        std::cout << "Can't open file " << input << std::endl;
        return;
    }

    int maxRho = std::sqrt(an_image.num_columns() * an_image.num_columns() + an_image.num_rows() * an_image.num_rows());
    output.AllocateSpaceAndSetSize(180 + 1, maxRho); 


    for (int i = 0; i < an_image.num_rows(); i++) {
        for (int j = 0; j < an_image.num_columns(); j++) {
            if (an_image.GetPixel(i, j) > 0) {
                for (int theta = 0; theta <= 180; theta++) {
                    int rho = std::round(j * std::cos(theta * M_PI / 180) + i * std::sin(theta * M_PI / 180));
                    if (rho < 0 || rho >= maxRho) continue;
                    int value = output.GetPixel(theta, rho) + 1;
                    output.SetPixel(theta, rho, value);
                }
            }
        }
    }
   /*
   
   
   
   std::ofstream outFile(outputText);
    if (!outFile) {
        std::cout << "Can't open file " << outputText << std::endl;
        return;
    }
   

    for (int i = 0; i < output.num_columns(); i++) {
        //outFile<<"\n";  // Add a new line after each row of pixels
        for (int j = 0; j < output.num_rows(); j++) {
            int pixel = output.GetPixel(i, j);
            //outFile << i << " " << j << " " << pixel;  // Save the data to the text file
            if (pixel > 255) {
                output.SetPixel(i, j, 255);
            }
        }
    }

    //outFile.close();
    output.SetNumberGrayLevels(255);
    WriteImage(outputBinary, output);
}
*/

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