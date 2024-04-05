#include "image.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>
#include <array>


using namespace ComputerVisionProjects; 

// Struct to store the albedos
struct albedos{
    int x;
    int y;
    double albedo;
};

/***
 * Calculate the inverse of a 3x3 matrix
*/
/**
 * @brief Calculate the inverse of a 3x3 matrix
 * @param m The matrix
 * @return The inverse of the matrix
*/
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
/**
 * @brief Computes the normal map and albedo map of an object given three images of the object under different lighting conditions
 * 
 * @param input_directions_filename 
 * @param input_object_image_1_filename 
 * @param input_object_image_2_filename 
 * @param input_object_image_3_filename 
 * @param step 
 * @param threshold 
 * @param output_normals_image_filename 
 * @param output_albedo_image_filename 
 */

void s3(std::string input_directions_filename, const std::array<std::string,3> &input_object_filenames, int step, int threshold, std::string output_normals_image_filename, std::string output_albedo_image_filename) {
    // Load the images
    Image object_image_1, object_image_2, object_image_3;
    if (!ReadImage(input_object_filenames[0].c_str(), &object_image_1)) {
        std::cerr << "Can't open file " << input_object_filenames[0].c_str() << std::endl;
        exit(1);
    }
    if (!ReadImage(input_object_filenames[1].c_str(), &object_image_2)) {
        std::cerr << "Can't open file " << input_object_filenames[1].c_str() << std::endl;
        exit(1);
    }
    if (!ReadImage(input_object_filenames[0].c_str(), &object_image_3)) {
        std::cerr << "Can't open file " << input_object_filenames[2].c_str() << std::endl;
        exit(1);
    }

    // Load the directions
    std::ifstream directions_file(input_directions_filename);
    if (!directions_file.is_open()) {
        std::cerr << "Can't open file " << input_directions_filename << std::endl;
        exit(1);
    }
    // Initialize the matrix
    std::vector<std::vector<double>> matrix;
    std::ifstream file(input_directions_filename);
    std::string line;
    // Read the matrix from the file
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        double value;

        while (ss >> value) {
            row.push_back(value);
        }

        matrix.push_back(row);
    }
    // Calculate the inverse of the matrix
    matrix = calculateInverse(matrix);
    Image output;
    // Initialize the albedos vector
    std::vector<albedos> albedoss;
    // Initialize the max albedo
    double max_albedo = 0;
    // Allocate space for the output image
    output.AllocateSpaceAndSetSize(object_image_1.num_rows(), object_image_1.num_columns());
    output.SetNumberGrayLevels(255);
    Image normals = object_image_1; 
    // Iterate over the pixels of the object images
    int first(0) , second(0), third(0);
    for(int i = 0; i < object_image_1.num_rows(); i++){
        for(int j = 0; j < object_image_1.num_columns();j++){
            // Check if the pixel is above the threshold
            if(object_image_1.GetPixel(i, j) > threshold && object_image_2.GetPixel(i, j) > threshold && object_image_3.GetPixel(i, j) > threshold){
                // Calculate the albedo
                first = object_image_1.GetPixel(i, j);
                second = object_image_2.GetPixel(i, j);
                third = object_image_3.GetPixel(i, j);
                //std::cout<<"First: "<<first<<" Second: "<<second<<" Third: "<<third<<std::endl;
                double n0 = first * matrix[0][0] + second * matrix[0][1] + third * matrix[0][2];
                double n1 = first * matrix[1][0] + second * matrix[1][1] + third * matrix[1][2];
                double n2 = first * matrix[2][0] + second * matrix[2][1] + third * matrix[2][2];
                double albedo = sqrt(n0*n0 + n1*n1 + n2*n2);
                //std::cout<<"Albedo: "<<albedo<<std::endl;
                if(albedo > max_albedo){
                    max_albedo = albedo;
                }
                // Store the albedo in the vector
                albedos temp;
                temp.x = i;
                temp.y = j;
                temp.albedo = albedo;
                albedoss.push_back(temp);
                // If is at the step, calculate the normal and store it in the normals image
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
    // Display the normalized albedo image
    for(auto& albedo : albedoss){
        output.SetPixel(albedo.x, albedo.y, albedo.albedo/max_albedo * 255);
    }
    // Write the images
    if (!ComputerVisionProjects::WriteImage(output_albedo_image_filename, output) && ComputerVisionProjects::WriteImage(output_normals_image_filename, normals) ) {
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
    std::array<std::string, 3> input_object_image_filenames = {argv[2], argv[3], argv[4]};
    int step = std::stoi(argv[5]);
    int threshold = std::stoi(argv[6]);
    std::string output_normals_image_filename(argv[7]);
    std::string output_albedo_image_filename(argv[8]);

    s3(input_directions_filename, input_object_image_filenames, step, threshold, output_normals_image_filename, output_albedo_image_filename);

    return 0;
}