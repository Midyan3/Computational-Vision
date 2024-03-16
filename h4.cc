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
     int label = 1;
    int row = static_cast<int>(Voting_Array_Image.num_rows());
    int col = static_cast<int>(Voting_Array_Image.num_columns());
    std::cout<<row<<std::endl;
    std::cout<<col<<std::endl;  
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
                Voting_Array_Image.SetPixel(i, j, labels[i][j] * 10);
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
    WriteImage("voting_array.pgm", Voting_Array_Image);
    
for (auto& [label, Obj] : Objects) {
    Obj.CentroidX = Obj.SumX / Obj.Area;
    Obj.CentroidY = Obj.SumY / Obj.Area; 
    double theta1 = Obj.CentroidX* M_PI / 180.0;  // Theta in radians
    int rho1 = static_cast<int>(Obj.CentroidY);

    int M = an_image->num_columns();  // Width of the image
    int N = an_image->num_rows();     // Height of the image

    // Using the line equation: rho = x * cos(theta) + y * sin(theta)
    // Solve for y when x = 0
    int y1 = (rho1 - 0 * cos(theta1)) / sin(theta1);

    // Solve for x when y = 0
    int x2 = (rho1 - 0 * sin(theta1)) / cos(theta1);

    // Solve for y when x = M - 1
    int y3 = (rho1 - (M) * cos(theta1)) / sin(theta1);

    // Solve for x when y = N - 1
    int x4 = (rho1 - (N) * sin(theta1)) / cos(theta1);

    // Check which points are within the boundaries of the image
    std::vector<std::pair<int, int>> validPoints;

    if (y1 >= 0 && y1 < N - 2) validPoints.emplace_back(0, y1);
    if (x2 >= 0 && x2 < M - 2) validPoints.emplace_back(x2, 0);
    if (y3 >= 0 && y3 < N - 2) validPoints.emplace_back(M - 1, y3);
    if (x4 >= 0 && x4 < M - 2) validPoints.emplace_back(x4, N - 1);

    // Draw line if exactly two points are within the image boundaries
    if (validPoints.size() == 2) {
        std::cout << "Drawing line for object " << label << std::endl;
        std::cout<< validPoints[0].first << " " << validPoints[0].second << " " << validPoints[1].first << " " << validPoints[1].second << std::endl;
        DrawLine(validPoints[0].second,validPoints[0].first , 
                validPoints[1].second , validPoints[1].first, 180, an_image);
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