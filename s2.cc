#include "image.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

//s2 {input parameters filename} {input sphere image 1 filename} {input sphere image 2 filename} {input sphere image 3 filename} {output directions filename}
using namespace ComputerVisionProjects;

void s2(std::string filename, std::string sphere1, std::string sphere2, std::string sphere3, std::string output){
    Image image1 , image2, image3;
    ReadImage(sphere1, &image1);
    ReadImage(sphere2, &image2);
    ReadImage(sphere3, &image3);
    std::vector<Image> images = {image1, image2, image3};
    double centerX, centerY;
    int radius;

    // Open the file for reading
    std::ifstream inputFile(filename);

    // Check if the file was successfully opened
    if (inputFile.is_open()) {
        // Read the values from the file
        inputFile >> centerX >> centerY >> radius;

        // Close the file
        inputFile.close();

        // Print the values
        std::cout << "Center X: " << centerX << std::endl;
        std::cout << "Center Y: " << centerY << std::endl;
        std::cout << "Radius: " << radius << std::endl;
    } else {
        std::cout << "Unable to open the file." << std::endl;
    }
    inputFile.close();
    std::ofstream outputFile(output);
    int max; 
    double z;  
    std::pair<double, double> maxCord;
    for(auto& img : images){
        z = 0;
        max = 0;
        for(int i = 0; i < img.num_rows(); i++){
            for(int j = 0; j < img.num_columns(); j++){
                if(img.GetPixel(i, j) > max){
                    max = img.GetPixel(i, j);
                    maxCord = {i, j};
                }
            }
        }
        double x = maxCord.second - centerX;
        double y = maxCord.first - centerY;
        z = sqrt(pow(radius, 2) - pow(x, 2) - pow(y, 2));
        double length = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));    
        outputFile << max * x/ length<< " " << max * y / length  << " " << max * z / length<< std::endl; 
        std::cout<<max * x/ length<< " " << max * y / length  << " " << max* z / length<< std::endl;
        
    }
    outputFile.close();

    
    
}

int main(int argc, char **argv){
    if(argc != 6){
        std::cout<<"Usage: "<<argv[0]<<" <filename> <sphere1> <sphere2> <sphere3> <output>\n";
        return 0;
    }
    std::string filename(argv[1]);
    std::string sphere1(argv[2]);
    std::string sphere2(argv[3]);
    std::string sphere3(argv[4]);
    std::string output(argv[5]);
    s2(filename, sphere1, sphere2, sphere3, output);
    return 0;
}