#include "image.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>



using namespace ComputerVisionProjects; 


struct albedos{
    int x;
    int y;
    double albedo;
};

std::vector<std::vector<double>> calculateInverse(const std::vector<std::vector<double>>& m) {
  double det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
                 m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
                 m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    double invdet = 1 / det;

    std::vector<std::vector<double>> minv(3, std::vector<double>(3));

    minv[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invdet;
    minv[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invdet;
    minv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invdet;
    minv[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invdet;
    minv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invdet;
    minv[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * invdet;
    minv[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invdet;
    minv[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * invdet;
    minv[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invdet;

    return minv;
}


void s3(std::string input_directions_filename, std::string input_object_image_1_filename, std::string input_object_image_2_filename, std::string input_object_image_3_filename, int step, int threshold, std::string output_normals_image_filename, std::string output_albedo_image_filename) {
    // Load the images
    Image object_image_1, object_image_2, object_image_3;
    if (!ReadImage(input_object_image_1_filename, &object_image_1)) {
        std::cerr << "Can't open file " << input_object_image_1_filename << std::endl;
        exit(1);
    }
    if (!ReadImage(input_object_image_2_filename, &object_image_2)) {
        std::cerr << "Can't open file " << input_object_image_2_filename << std::endl;
        exit(1);
    }
    if (!ReadImage(input_object_image_3_filename, &object_image_3)) {
        std::cerr << "Can't open file " << input_object_image_3_filename << std::endl;
        exit(1);
    }

    // Load the directions
    std::ifstream directions_file(input_directions_filename);
    if (!directions_file.is_open()) {
        std::cerr << "Can't open file " << input_directions_filename << std::endl;
        exit(1);
    }
    std::vector<std::vector<double>> matrix;
    std::ifstream file(input_directions_filename);
    std::string line;

    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        double value;

        while (ss >> value) {
            row.push_back(value);
        }

        matrix.push_back(row);
    }
    for(auto& row : matrix){
        for(auto& val : row){
            std::cout<<val<<" ";
        }
        std::cout<<std::endl;
    }

    matrix = calculateInverse(matrix);
    for(auto& row : matrix){
        for(auto& val : row){
            std::cout<<val<<" ";
        }
        std::cout<<std::endl;
    }
    Image output;
    std::vector<albedos> albedoss;
    double max_albedo = 0;
    int count = 0;
    output.AllocateSpaceAndSetSize(object_image_1.num_rows(), object_image_1.num_columns());
    output.SetNumberGrayLevels(255);
    Image normals = object_image_1; 
    int first(0) , second(0), third(0);
    for(int i = 0; i < object_image_1.num_rows(); i++){
        for(int j = 0; j < object_image_1.num_columns();j++){
            if(object_image_1.GetPixel(i, j) > threshold && object_image_2.GetPixel(i, j) > threshold && object_image_3.GetPixel(i, j) > threshold){
                first = object_image_1.GetPixel(i, j);
                second = object_image_2.GetPixel(i, j);
                third = object_image_3.GetPixel(i, j);
                //std::cout<<"First: "<<first<<" Second: "<<second<<" Third: "<<third<<std::endl;
                double n0 = first * matrix[0][0] + second * matrix[0][1] + third * matrix[0][2];
                double n1 = first * matrix[1][0] + second * matrix[1][1] + third * matrix[1][2];
                double n2 = first * matrix[2][0] + second * matrix[2][1] + third * matrix[2][2];
                double albedo = sqrt(n0*n0 + n1*n1 + n2*n2);
                std::cout<<"Albedo: "<<albedo<<std::endl;
                if(albedo > max_albedo){
                    max_albedo = albedo;
                }
                albedos temp;
                temp.x = i;
                temp.y = j;
                temp.albedo = albedo;
                albedoss.push_back(temp);
                if(i % step == 0 && j % step == 0){
                
                
                n0 = n0/albedo;
                n1 = n1/albedo;
                n2 = n2/albedo;
                normals.SetPixel(i -1 , j - 1 , 255 );
                normals.SetPixel(i -1 , j , 255 );
                normals.SetPixel(i  , j + 1 , 255 );
                normals.SetPixel(i + 1 , j , 255 );
                DrawLine(i, j, i + n0*10, j + n1*10, 255, &normals);
                normals.SetPixel(i, j, 0);
                /*
                dont display, save them in vector and then find biigest one divide all by the biggest one and then mutpliy 255
                
                */
                } 
            }
        }
    }
    for(auto& albedo : albedoss){
        output.SetPixel(albedo.x, albedo.y, albedo.albedo/max_albedo * 255);
    }
    ComputerVisionProjects::WriteImage(output_normals_image_filename, normals); 
    if (!ComputerVisionProjects::WriteImage(output_albedo_image_filename, output) ) {
        std::cerr << "Can't write to file " << output_albedo_image_filename << std::endl;
        exit(1);
    }
}



int main(int argc, char **argv) {
    if (argc != 9) {
        std::cerr << "Usage: " << argv[0] << " {input directions filename} {input object image 1 filename} {input object image 2 filename} {input object image 3 filename} {input step parameter (integer greater than 0)} {input threshold parameter (integer greater than 0)} {output normals image filename} {output albedo image filename}" << std::endl;
        return 1;
    }

    std::string input_directions_filename(argv[1]);
    std::string input_object_image_1_filename(argv[2]);
    std::string input_object_image_2_filename(argv[3]);
    std::string input_object_image_3_filename(argv[4]);
    int step = std::stoi(argv[5]);
    int threshold = std::stoi(argv[6]);
    std::string output_normals_image_filename(argv[7]);
    std::string output_albedo_image_filename(argv[8]);

    s3(input_directions_filename, input_object_image_1_filename, input_object_image_2_filename, input_object_image_3_filename, step, threshold, output_normals_image_filename, output_albedo_image_filename);

    return 0;
}