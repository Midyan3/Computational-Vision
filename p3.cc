#include <vector>
#include <map>
#include <iostream>
#include <string> 
#include "image.h"
#include <cmath>
#include <fstream>

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

using namespace ComputerVisionProjects; 

void p3(std::string input, std::string ObjectOutput, std::string output){
    std::map<int, Moments> Objects;
    Image *an_image = new Image();
    std::ofstream ObjectDesc(ObjectOutput+ ".txt");
    ReadImage(input, an_image);
    int row = static_cast<int>(an_image->num_rows());
    int col = static_cast<int>(an_image->num_columns());
    for(int x = 0; x < row; x++){
        for(int y = 0; y < col; y++){
            if(an_image->GetPixel(x, y) == 0 ) continue;
            auto& Obj = Objects[an_image->GetPixel(x, y)];
            Obj.Area += 1;
            Obj.SumX += x;
            Obj.SumY += y;
            Obj.a_prime += x * x;
            Obj.c_prime += y * y;
            Obj.b_prime += 2 * x * y;
        }
    }
    int labels = 0;

    for(auto& [label, Obj] : Objects){
        Obj.CentroidX = Obj.SumX / Obj.Area;
        Obj.CentroidY = Obj.SumY / Obj.Area;
        
        // Calculate central moments from raw moments
        Obj.a = Obj.a_prime - Obj.CentroidX * Obj.CentroidX * Obj.Area; 
        Obj.c = Obj.c_prime - Obj.CentroidY * Obj.CentroidY * Obj.Area;
        Obj.b = Obj.b_prime -  2 * Obj.CentroidX * Obj.CentroidY * Obj.Area;
        Obj.Theta = atan2( Obj.b, Obj.a - Obj.c) / 2.0;
        Obj.e_min = Obj.a*sin(Obj.Theta)*sin(Obj.Theta) - Obj.b*sin(Obj.Theta)*cos(Obj.Theta) + Obj.c*cos(Obj.Theta)*cos(Obj.Theta);
        Obj.e_max = Obj.a*sin(Obj.Theta + M_PI/2)*sin(Obj.Theta + M_PI/2) - Obj.b*sin(Obj.Theta + M_PI/2)*cos(Obj.Theta + M_PI/2) + Obj.c*cos(Obj.Theta + M_PI/2)*cos(Obj.Theta + M_PI/2);
        Obj.row = Obj.CentroidY*cos(Obj.Theta) - Obj.CentroidX*sin(Obj.Theta);
        DrawLine(Obj.CentroidX, Obj.CentroidY, Obj.CentroidX + 30*cos(Obj.Theta), Obj.CentroidY + 30*sin(Obj.Theta), 255, an_image);
        ObjectDesc<<labels<<" "<< Obj.CentroidX<<" "<<Obj.CentroidY<<" "<<Obj.e_min<<" "<<Obj.Area<<" "<< Obj.e_min/Obj.e_max<<" "<<Obj.Theta*180/M_PI<<"\n";
        labels++; 
    }
    ObjectDesc.close();

    WriteImage(output + ".pmg", *an_image);

}


int main(int argc, char **argv){
    if(argc != 4){
        std::cout<<"Usage: "<<argv[0]<<" <input_image.pgm> <ObjectOutput.pgm> <output_image.pgm>\n";
        return 0;
    }
    std::string input(argv[1]);
    std::string ObjectOutput(argv[2]);
    std::string output(argv[3]);
    p3(input, ObjectOutput, output);
    return 0;
}
