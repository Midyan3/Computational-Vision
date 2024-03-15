#include "image.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <map>
#include "DisjSets.h"
using namespace ComputerVisionProjects;

struct Moments{
    double Area = 0; // Total area (or count) of the object pixels
    double SumX = 0; // Sum of x coordinates of all pixels
    double SumY = 0; // Sum of y coordinates of all pixels
    double CentroidX = 0; 
    double CentroidY = 0; 
};
void h4(std::string OrginalImage, std::string Voting_Array, int threshold, std::string OutputImage) {
     Image an_image;

    if (!ReadImage(OrginalImage, &an_image)) {
        std::cout << "Can't open file " << OrginalImage << std::endl;
        return;
    }

    std::ifstream file(Voting_Array);
    if (!file) {
        std::cout << "Can't open file " << Voting_Array << std::endl;
        return;
    }

    int theta, rho, value;
    int rhoMax = std::sqrt(std::pow(an_image.num_columns(), 2) + std::pow(an_image.num_rows(), 2));

    Image Voting_Array_Image;
    Voting_Array_Image.AllocateSpaceAndSetSize(181, rhoMax + 1);
    Voting_Array_Image.SetNumberGrayLevels(255);  // Assuming the voting array uses grayscale values

    while (file >> theta >> rho >> value) {
        if (value > threshold){
            Voting_Array_Image.SetPixel(theta, rho, 255);
        } 
    }

    file.close();
     int label = 1;
    int row = static_cast<int>(Voting_Array_Image.num_rows());
    int col = static_cast<int>(Voting_Array_Image.num_columns());
    DisjSets Equal(row*col );
    std::vector<std::vector<int>> labels(row, std::vector<int>(col, 0));
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (Voting_Array_Image.GetPixel(i, j) == 255 ) { // Foreground pixel
            std::vector<int> neighbors = {
                (j > 0) ? labels[i][j-1] : 0,
               (i > 0) ? labels[i-1][j] : 0,
               (i > 0 && j > 0) ? labels[i-1][j-1] : 0
            };

            int max = std::max(std::max(neighbors[0], neighbors[1]), neighbors[2]);
            if(max == 0){
                labels[i][j] = label;
                label++;
            }
            else if(neighbors[2] != 0){
                labels[i][j] = neighbors[2];
            }
            else if((neighbors[0] != 0 && neighbors[1] == 0) || (neighbors[0] == 0 && neighbors[1] != 0)){
                labels[i][j] = std::max(neighbors[0], neighbors[1]);
            }
            else{
                labels[i][j] = std::min(neighbors[0], neighbors[1]);
                const int f0 = Equal.find(neighbors[0]);
                const int f1 = Equal.find(neighbors[1]);
                if(f0 != f1){
                  Equal.unionSets(f0, f1);                  
                }

            }
        }
    }

}
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (Voting_Array_Image.GetPixel(i, j) == 255 ) { 
                labels[i][j] = Equal.find(labels[i][j]);                
                
            }
        }
    } 

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(labels[i][j] != 0){ 
                Voting_Array_Image.SetPixel(i, j, labels[i][j] * 10 + 50);
            }
           
        }
    }
     std::map<int, Moments> Objects;
    for(int x = 0; x < row; x++){
        for(int y = 0; y < col; y++){
            if(Voting_Array_Image.GetPixel(x, y) == 0 ) continue;
            auto& Obj = Objects[Voting_Array_Image.GetPixel(x, y)];
            Obj.Area += 1;
            Obj.SumX += x;
            Obj.SumY += y;
        }
    }
for (auto& [label, Obj] : Objects) {
    Obj.CentroidX = Obj.SumX / Obj.Area;  // Average theta in degrees
    Obj.CentroidY = Obj.SumY / Obj.Area;  // Average rho
    std::cout << label << " Theta " << Obj.CentroidX << " Rho " << static_cast<int>(Obj.CentroidY) << std::endl;

    double theta1 = round(Obj.CentroidX) * M_PI / 180.0;  // Convert theta to radians
    int rho1 = static_cast<int>(Obj.CentroidY);

    int width = an_image.num_columns();
    int height = an_image.num_rows();

    // Calculate intersection points with the image boundaries
    double sin_theta = sin(theta1);
    double cos_theta = cos(theta1);

    std::vector<std::pair<int, int>> points;

    // Check for division by zero for horizontal and vertical lines
    if (std::abs(cos_theta) > 1e-6) {
        int x0 = static_cast<int>(rho1 / cos_theta);  // Intersection with top boundary (y = 0)
        int x1 = static_cast<int>((rho1 - height * sin_theta) / cos_theta);  // Bottom boundary (y = height)

        if (x0 >= 0 && x0 <= width) points.emplace_back(x0, 0);
        if (x1 >= 0 && x1 <= width) points.emplace_back(x1, height);
    }

    if (std::abs(sin_theta) > 1e-6) {
        int y0 = static_cast<int>(rho1 / sin_theta);  // Left boundary (x = 0)
        int y1 = static_cast<int>((rho1 - width * cos_theta) / sin_theta);  // Right boundary (x = width)

        if (y0 >= 0 && y0 <= height) points.emplace_back(0, y0);
        if (y1 >= 0 && y1 <= height) points.emplace_back(width, y1);
    }

    // Now, you have the points where the line intersects the image boundaries
    // You can draw the line if there are at least two intersection points
    if (points.size() >= 2) {
        std::cout << "Drawing line " << label << " From " << points[0].first << " " << points[0].second << " To " << points[1].first << " " << points[1].second << std::endl;
        DrawLine(points[0].first, points[0].second, points[1].first, points[1].second, 155, &an_image);
    }
}
    
    if (!WriteImage(OutputImage, an_image)) {
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