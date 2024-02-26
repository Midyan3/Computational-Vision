#include "image.h"
#include <string>
#include <iostream>

using namespace ComputerVisionProjects;
using namespace std;

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



int main(int argc, char ** argv) {

    if (argc!=4) {
      printf("Usage: %s input_file threshold output_file\n", argv[0]);
      return 0;
    }
    
    const string input_filename(argv[1]);
    const int threshold = atoi(argv[2]);
    const string output_filename(argv[3]);
    p1(input_filename, threshold, output_filename);    
    return 0;
}

