#include "image.h"
#include <iostream>
#include <string>
#include <fstream>
#include "DisjSets.h"
#include <limits>    
#include <map>
using namespace ComputerVisionProjects; 

struct Moments{
    double Area = 0; // Total area (or count) of the object pixels
    double SumX = 0; // Sum of x coordinates of all pixels
    double SumY = 0; // Sum of y coordinates of all pixels
    double a_prime = 0; // Raw second moment (m20), sum of x^2 for all pixels
    double c_prime = 0; // Raw second moment (m02), sum of y^2 for all pixels
    double b_prime = 0; //- Raw mixed moment (m11), sum of x*y for all pixels
    double CentroidX = 0; // x coordinate of the centroid
    double CentroidY = 0; // y coordinate of the centroid
    double Theta = 0; // Orientation angle in radians
    double a = 0; // Central second moment along the x-axis
    double c = 0; // Central second moment along the y-axis
    double b = 0; // Central mixed moment
    double e_min = 0; // Smaller eigenvalue
    double e_max = 0; // Larger eigenvalue
    double row = 0; // Row of the object
};


void s1(std::string input, int threshold, std::string output_file){
Image img;
ReadImage(input, &img);

int rows = img.num_rows();
int cols = img.num_columns();

int minLeft = INT32_MAX, minTop = INT32_MAX, maxRight = 0, maxBottom = 0;

for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        if (img.GetPixel(i, j) < threshold) {
            img.SetPixel(i, j, 0);
 
        } else {           
            if (j < minLeft) {
                minLeft = j;
            }
            if (j > maxRight) {
                maxRight = j;
            }
            if (i < minTop) {
                minTop = i;
            }
            if (i > maxBottom) {
                maxBottom = i;
            }
            img.SetPixel(i, j, 255);
        }
    }
}


    int label = 1;
    int row = static_cast<int>(img.num_rows());
    int col = static_cast<int>(img.num_columns());
    DisjSets Equal(row*col );
    std::vector<std::vector<int>> labels(row, std::vector<int>(col, 0));
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (img.GetPixel(i, j) == 255 ) { // Foreground pixel
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
            if (img.GetPixel(i, j) == 255 ) { // Foreground pixel
                labels[i][j] = Equal.find(labels[i][j]);                
                
            }
        }
    } 
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            img.SetPixel(i, j, labels[i][j] * 10);
            
        }
    }

    std::map<int, Moments> Objects;
    std::ofstream ObjectDesc(output_file);
    for(int x = 0; x < row; x++){
        for(int y = 0; y < col; y++){
            if(img.GetPixel(x, y) == 0 ) continue;
            auto& Obj = Objects[img.GetPixel(x, y)];
            Obj.Area += 1;
            Obj.SumX += x;
            Obj.SumY += y;
        }
    }

    for(auto& [label, Obj] : Objects){
        Obj.CentroidX = Obj.SumX / Obj.Area;
        Obj.CentroidY = Obj.SumY / Obj.Area;
        ObjectDesc<<Obj.CentroidY<<" "<< Obj.CentroidX<<" "<< (abs(maxRight - minLeft) + abs(maxBottom - minTop)) / 4;
    } 
    ObjectDesc.close();
    std::cout << "MinLeft: " << minLeft << " MinTop: " << minTop << " MaxRight: " << maxRight << " MaxBottom: " << maxBottom << std::endl;


}


int main(int argc, char **argv){
    if(argc != 4){
        std::cout << "Usage: <input file> <threshold> <output file>" << std::endl;
        return 0;
    }
    std::string input_file = argv[1];
    int threshold = std::stoi(argv[2]);
    std::string output_file = argv[3];
    s1(input_file, threshold, output_file);
    return 0;
}