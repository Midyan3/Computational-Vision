#include <vector>
#include <map>
#include <iostream>
#include <string> 
#include "image.h"
#include <cmath>
#include <fstream>
#include <sstream>


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

double CompareMoments(const Moments& obj1, const Moments& obj2) {
    double eRatio1 = obj1.e_min / obj1.e_max;
    double eRatio2 = obj2.e_min / obj2.e_max;
    double eRatioDiff = std::abs(eRatio1 - eRatio2);
    double areaDiff = std::abs(obj1.Area - obj2.Area) / std::max(obj1.Area, obj2.Area);
    double similarityScore =   eRatioDiff + areaDiff;
    std::cout<<"Similarity Score: "<<similarityScore<<"\n";
    return similarityScore;
}

void p4(std::string input, std::string ObjectDesc, std::string output){
std::map<int, Moments> ObjectsFromTxt;
    std::map<int, Moments> Objects;
    Image *an_image = new Image();
    double threshold = 0.20;
    ReadImage(input, an_image);
    int row = static_cast<int>(an_image->num_rows());
    int col = static_cast<int>(an_image->num_columns());
    std::ifstream ObjectDes(ObjectDesc); // Adjust the filename as needed

    if (!ObjectDes.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    std::string line;
    while (getline(ObjectDes, line)) {
        std::istringstream iss(line);
        int label;
        Moments moment;
        double ratio, thetaDegrees;

        if (!(iss >> label >> moment.CentroidX >> moment.CentroidY >> moment.e_min >> moment.Area >> ratio >> thetaDegrees)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue; // Skip malformed lines
        }

        // Convert theta from degrees to radians and compute e_max
        moment.Theta = thetaDegrees * M_PI / 180.0;
        moment.e_max = moment.e_min / ratio;

        ObjectsFromTxt[label] = moment;
    }

    ObjectDes.close();

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
        labels++; 
    }

    for(const auto& [label, ObjTxt] : ObjectsFromTxt){
        for(const auto& [label2, Obj] : Objects){
            if(CompareMoments(ObjTxt, Obj) < threshold){
                DrawLine(Obj.CentroidX, Obj.CentroidY, Obj.CentroidX + 30*cos(Obj.Theta), Obj.CentroidY + 30*sin(Obj.Theta), 255, an_image);
            }
        }
    }
    WriteImage(output, *an_image);

}

int main(int argc, char **argv){
    if(argc != 4){
        std::cout<<"Usage: "<<argv[0]<<" <input_image.pgm> <ObjectOutput.pgm> <output_image.pgm>\n";
        return 0;
    }
    std::string input(argv[1]);
    std::string ObjectDesc(argv[2]);
    std::string output(argv[3]);
    p4(input, ObjectDesc, output);
    return 0;
}
