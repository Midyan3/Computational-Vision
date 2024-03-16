#include "image.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace ComputerVisionProjects;

void h2(const string &input, int threshold,  const string &output) {
    Image an_image;
    Image output_image;
    if (!ReadImage(input, &an_image)) {
        cout << "Can't open file " << input << endl;
        return;
    }

    output_image.AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
    output_image.SetNumberGrayLevels(255);

    vector<vector<int>> sobelX = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    vector<vector<int>> sobelY = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < an_image.num_columns(); i++) {
        for (int j = 0; j < an_image.num_rows(); j++) {
            int sumX = 0;
            int sumY = 0;
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    int x = i + k - 1;
                    int y = j + l - 1;
                    if (x >= 0 && x < an_image.num_columns() && y >= 0 && y < an_image.num_rows()) {
                        sumX += an_image.GetPixel(y, x) * sobelX[k][l];
                        sumY += an_image.GetPixel(y, x) * sobelY[k][l];
                    }
                }
            }
            int magnitude = static_cast<int>(sqrt(sumX * sumX + sumY * sumY));
            output_image.SetPixel(j, i, magnitude > threshold ? 255 : 0);
        }
    }

    WriteImage(output, output_image);
}

int main(int argc, char **argv) {

    if (argc != 4) {
        std::cout <<
            "Usage: " << argv[0] << " <input_image> <threshold> <output_image>" << std::endl;
        return 0;
    }

    std::string input_image(argv[1]);
    int threshold = std::stoi(argv[2]);
    std::string output_image(argv[3]);

    h2(input_image, threshold, output_image);
    return 0;
}