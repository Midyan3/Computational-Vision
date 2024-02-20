#include "image.h"
#include <string>
#include <iostream>
#include <vector>
#include "DisjSets.h"
#include <algorithm>

using namespace ComputerVisionProjects;

/*
Program 2 Write a labeling program named p2 that segments a binary image into several
connected regions:
p2 {input binary image} {output labeled image}.
A good choice is the sequential labeling algorithm described in the class. Note that you
may have to make two passes of the image to resolve possible equivalences in the labels.
In the ”labeled” output image each object region should be painted with a different
gray–level: the gray–level assigned to an object is its label. The labeled image can be
displayed to check the results produced by your program. Note that your program must
be able to produce correct results given any binary image. You can test it on the images
given to you. Apply program p2 to the binary version of image two objects.pgm. (12
points)

*/

void p2(std::string input_file ,std::string output_file){
    Image *anImage = new Image();
    ReadImage(input_file, anImage);
    int label = 1;
    int row = static_cast<int>(anImage->num_rows());
    int col = static_cast<int>(anImage->num_columns());
    std::cout<<"Got here 7  \n";
    DisjSets Equal(row*col );
    std::vector<std::vector<int>> labels(row, std::vector<int>(col, 0));
    std::cout<<"Got here 8  \n";
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (anImage->GetPixel(i, j) == 255 ) { // Foreground pixel
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
                std::cout<<neighbors[0] << " " << neighbors[1] << std::endl;
                labels[i][j] = std::max(neighbors[0], neighbors[1]);
            }
            else{
                labels[i][j] = std::min(neighbors[0], neighbors[1]);
                if(neighbors[0] != neighbors[1]){
                    Equal.unionSets(neighbors[0] , neighbors[1]);
                }

            }
        }
    }

}
    std::cout<<"Got here 9  \n";
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (anImage->GetPixel(i, j) == 255 ) { // Foreground pixel
                labels[i][j] = Equal.find(labels[i][j]);
                
            }
        }
    } 
    int max = 0;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            max = std::max(max, labels[i][j]);
            anImage->SetPixel(i, j, labels[i][j]);
        }
    }
    std::cout<<"Max label is: "<<max<<std::endl;
    ComputerVisionProjects::WriteImage(output_file, *anImage);
}

/*

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (anImage->GetPixel(i, j) == 255 ) { // Foreground pixel
                int leftLabel = (j > 0) ? labels[i][j-1] : 0;
                int topLabel = (i > 0) ? labels[i-1][j] : 0;
                int topLeftLabel = (i > 0 && j > 0) ? labels[i-1][j-1] : 0;

                // No neighbors are labeled
                if (leftLabel == 0 && topLabel == 0 && topLeftLabel == 0) {
                    labels[i][j] = label; // Assign a new label
                    Equal.unionSets(label, label); // Union the label with itself
                    label++;
                } 
                // Diagonal neighbor is labeled
                else if (topLeftLabel != 0) {
                    labels[i][j] = topLeftLabel;
                } 
                // One or both of the other neighbors are labeled
                else {
                    if (leftLabel != 0 && topLabel != 0) {
                        labels[i][j] = min(leftLabel, topLabel);
                        if (leftLabel != topLabel) {
                            Equal.unionSets(leftLabel, topLabel); // Union different labels
                        }
                    } else if (leftLabel != 0) { // Only the left neighbor is labeled
                        labels[i][j] = leftLabel;
                    } else if (topLabel != 0) { // Only the top neighbor is labeled
                        labels[i][j] = topLabel;
                    }
                }
            }
        }
    }
    for(auto n : labels){
        for(auto m : n){
            std::cout<< m << " ";
        }
        std::cout<<std::endl;
    }
*/
int main(){
    std::string input_file = "two_object_BW1.pgm";// Changed to a distinct input file name
    std::string output_file = "swb11.pmg"; // Changed to a distinct output file name
    p2(input_file, output_file);
    return 0;
}