#include "image.h"
#include <string>
#include <iostream>

using namespace ComputerVisionProjects;


void p1(std::string input_file, int threeshold ,std::string output_file){
    std::cout<< "Got to p1\n"<<std::endl;
    Image *anImage = new Image(); 
    std::cout<< "Got to 2\n"<<std::endl;
    if(!ReadImage(input_file, anImage)){
        std::cout<< "Error reading the image\n"<<std::endl;
        return;
    }
    std::cout<< "Got to 3\n"<<std::endl;
    for (size_t i = 0; i < anImage->num_rows(); ++i)
        for (size_t j = 0; j < anImage->num_columns(); ++j){
            if(anImage->GetPixel(i,j) > threeshold){
                anImage->SetPixel(i,j, 255);
            }else{
                anImage->SetPixel(i,j, 1);
            }
        }
    std::cout<< "Got to 4\n"<<std::endl;
    WriteImage(output_file, *anImage);
    std::cout<< "Got to 5\n"<<std::endl;
}



  int main() {
    std::string input_file = "many_objects_1.pgm"; // Changed to a distinct input file name
    std::string output_file = "two_object_BW1.pgm"; // Changed to a distinct output file name
    p1(input_file, 128, output_file);    
    return 0;
}

