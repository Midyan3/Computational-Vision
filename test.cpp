#include "DisjSets.h"
#include <iostream>
using namespace std;
int main() {
  DisjSets ds(10);
  ds.unionSets(1, 2);
  ds.unionSets(2, 3);
  ds.unionSets(4, 5);
  ds.unionSets(6, 7);
  ds.unionSets(5, 6);
  ds.unionSets(3, 7);
  ds.unionSets(7, 7);
  
    for (int i = 1; i < 8; i++) {
        std::cout << ds.find(i) << std::endl;
    }

  return 0;
}