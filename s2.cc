#include "image.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <array>

//s2 {input parameters filename} {input sphere image 1 filename} {input sphere image 2 filename} {input sphere image 3 filename} {output directions filename}
using namespace ComputerVisionProjects;

/**
 * @brief Computes the surface normals of an object given three images of the object under different lighting conditions
 * 
 * @param filename 
 * @param sphere1 
 * @param sphere2 
 * @param sphere3 
 * @param output 
 */
void s2(std::string filename,  const std::array<std::string, 3>& input_sphere_filenames, std::string output){
    // Read the images and making sure to not hardcode the number of images
    Image image1 , image2, image3;
    ReadImage(input_sphere_filenames[0].c_str(), &image1);
    ReadImage(input_sphere_filenames[1].c_str(), &image2);
    ReadImage(input_sphere_filenames[2].c_str(), &image3);
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
    // Initialize the variables
    int max; 
    double z;  
    // Initialize the pair to store the coordinates of the maximum pixel
    std::pair<double, double> maxCord;
    // Iterate through the images
    for(auto& img : images){
        // Initialize the variables to zero
        z = 0;
        max = 0;
        // Iterate through the image to find the maximum pixel value
        for(int i = 0; i < img.num_rows(); i++){
            for(int j = 0; j < img.num_columns(); j++){
                if(img.GetPixel(i, j) > max){
                    max = img.GetPixel(i, j);
                    maxCord = {i, j};
                }
            }
        }
        // Calculate x - x0, y - y0, z - z0
        double x =  maxCord.first- centerX;
        double y = maxCord.second - centerY;
        z = sqrt(pow(radius, 2) - pow(x, 2) - pow(y, 2));
        // Calculate the length
        double length = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); 
        // Normalize the values   
        outputFile <<  max * x/ length  << " " <<max * y / length<< " " << max * z / length<< std::endl;  
    }
    // Close the file
    outputFile.close();
}

int main(int argc, char **argv){
    if(argc != 6){
        std::cout<<"Usage: "<<argv[0]<<" <filename> <sphere1> <sphere2> <sphere3> <output>\n";
        return 0;
    }
    std::string filename = argv[1];
    std::array<std::string, 3> spheres = {argv[2], argv[3], argv[4]};
    std::string output = argv[5];
    s2(filename, spheres, output);
    return 0;
}