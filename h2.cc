#include "image.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;

void h2(std::string input, std::string output){
    Image an_image;
    Image output_image;
    if (!ReadImage(input, &an_image)) {
        std::cout <<"Can't open file " << input << std::endl;
        return;
    }
    output_image.AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
    for(int i = 0; i < an_image.num_columns(); i++){
        for(int j = 0; j < an_image.num_rows(); j++){
            output_image.SetPixel(j, i, 0);
        }
    }

    std::vector<std::vector<int>> sobetX = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    std::vector<std::vector<int>> sobetY = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for(int i = 0; i < an_image.num_columns(); i++){
        for(int j = 0; j < an_image.num_rows(); j++){
            int sumX = 0;
            int sumY = 0;
            for(int k = 0; k < 3; k++){
                for(int l = 0; l < 3; l++){
                    int x = i + k - 1;
                    int y = j + l - 1;
                    if(x >= 0 && x < an_image.num_columns() && y >= 0 && y < an_image.num_rows()){
                        sumX += an_image.GetPixel(y, x) * sobetX[k][l];
                        sumY += an_image.GetPixel(y, x) * sobetY[k][l];
                    }
                }
            }
            int sum = std::sqrt(sumX * sumX + sumY * sumY);
            if(sum > 255){
                sum = 255;
            }
            output_image.SetPixel(j, i, sum);
        }
    }

    for(int i = 0; i < an_image.num_columns(); i++){
        for(int j = 0; j < an_image.num_rows(); j++){
            if(output_image.GetPixel(j, i) > 60){
                output_image.SetPixel(j, i, 255);
            }
            else{
                output_image.SetPixel(j, i, 0); 
            } 
        }
    }
    output_image.SetNumberGrayLevels(255);
    ComputerVisionProjects::WriteImage(output, output_image);
}

int main(int argc, char **argv){
    if(argc != 3){
        std::cout<<"Usage: "<<argv[0]<<" <input_image.pgm> <output_image.pgm>\n";
        return 0;
    }
    std::string input(argv[1]);
    std::string output(argv[2]);
    h2(input, output);
    return 0;
}
