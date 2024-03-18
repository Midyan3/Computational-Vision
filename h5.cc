#include "image.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <map>
#include "DisjSets.h"
using namespace ComputerVisionProjects;

void h4(std::string OrginalImage, std::string Voting_Array, int threshold, std::string OutputImage) {
     Image *an_image = new Image();

    if (!ReadImage(OrginalImage, an_image)) {
        std::cout << "Can't open file " << OrginalImage << std::endl;
        return;
    }

    std::ifstream file(Voting_Array);
    if (!file) {
        std::cout << "Can't open file " << Voting_Array << std::endl;
        return;
    }

    int theta, rho, value;
    int rhoMax = std::sqrt(std::pow(an_image->num_columns(), 2) + std::pow(an_image->num_rows(), 2));

    Image Voting_Array_Image;
    Voting_Array_Image.AllocateSpaceAndSetSize(181, rhoMax + 1);
    Voting_Array_Image.SetNumberGrayLevels(255);  // Assuming the voting array uses grayscale values

    WriteImage("voting_array.pgm", Voting_Array_Image);
    while (file >> theta >> rho >> value) {
        if (value > threshold){
            Voting_Array_Image.SetPixel(theta, rho, 255);
        } 
    }
    file.close();
    
for (int i = 0; i < Voting_Array_Image.num_rows(); i++) {
    for (int j = 0; j < Voting_Array_Image.num_columns(); j++) {
        if (Voting_Array_Image.GetPixel(i,j ) == 255) {
            int rho1 = j;
            double theta1 = i * M_PI / 180.0;  // Theta in radians

            int M = an_image->num_columns();  // Width of the image
            int N = an_image->num_rows();     // Height of the image

            // Using the line equation: rho = x * cos(theta) + y * sin(theta)
            // Solve for y when x = 0
            int y1 = rho1 / sin(theta1);

            // Solve for x when y = 0
            int x2 = rho1 / cos(theta1);

            // Solve for y when x = M - 1
            int y3 = (rho1 - (M) * cos(theta1)) / sin(theta1);

            // Solve for x when y = N - 1
            int x4 = (rho1 - (N) * sin(theta1)) / cos(theta1);

            // Check which points are within the boundaries of the image
            std::vector<std::pair<int, int>> validPoints;

            if (y1 >= 0 && y1 < N - 2) validPoints.emplace_back(0, y1);
            if (x2 >= 0 && x2 < M - 2) validPoints.emplace_back(x2, 0);
            if (y3 >= 0 && y3 < N - 2) validPoints.emplace_back(M -1 , y3);
            if (x4 >= 0 && x4 < M - 2) validPoints.emplace_back(x4, N -1 );

            // Draw line if exactly two points are within the image boundaries
            if (validPoints.size() == 2) {
                std::cout<< validPoints[0].first << " " << validPoints[0].second << " " << validPoints[1].first << " " << validPoints[1].second << std::endl;
                DrawLine(validPoints[0].second,validPoints[0].first , 
                        validPoints[1].second , validPoints[1].first, 180, an_image);
            }

        }
    }

    
    
    }


    if (!WriteImage(OutputImage, *an_image)) {
        std::cout << "Can't write to file " << OutputImage << std::endl;
    }
    if (!WriteImage(OutputImage, *an_image)) {
            std::cout << "Can't write to file " << OutputImage << std::endl;
    }
    
}

int main(int argc, char **argv) {

    if (argc != 5) {
        std::cout <<
            "Usage: " << argv[0] << " <input_image> <voting_array> <threshold> <output_image>" << std::endl;
        return 0;
    }

    std::string input_image(argv[1]);
    std::string voting_array(argv[2]);
    int threshold = std::stoi(argv[3]);
    std::string output_image(argv[4]);

    h4(input_image, voting_array, threshold, output_image);
    return 0;
}