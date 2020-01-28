#include "stdafx.h"
#include "VoxelOctTree.hpp"

#include <math.h>
#include <iostream>
#include <string>

using namespace VOXEL_OCTTREE;

int main(int argc, char* argv[])
{
  try {
    std::ios::sync_with_stdio(false);

    VoxelOctTree root;
    std::string fileName;
    unsigned int discr;

    if (argc <= 1) {
      std::cout << "Input name of the file with point cloud:\n";    // transformer.xyz
      std::cin >> fileName;
    }
    else {
      fileName = static_cast<std::string>(argv[1]);
      std::cout << "File with point cloud is " << fileName << std::endl;
    }

    if (argc <= 2) {
      std::cout << "Input degree of discretization (number of levels in voxel octtree):\n";    // 8
      std::cin >> discr;
    }
    else {
      discr = atoi(argv[2]);
      std::cout << "Degree of discretization (number of levels in voxel octtree) " << discr << std::endl;
    }
  
    std::string repeat;
    Ray ray;

    root.CreateSvoFromPointCloud(fileName, discr);
    do {
      std::cout << "Do you want to intersect the voxel octtree with a ray? (y/n)\n";
      std::cin >> repeat;
      if (repeat == "y") {
        std::cout << "Input 3 coordinates of ray initial point:\n";    // 0 75 11
        std::cin >> ray.start[0] >> ray.start[1] >> ray.start[2];
        std::cout << "Input 3 coordinates of ray direction:\n";    // 1 0 0
        std::cin >> ray.direction[0] >> ray.direction[1] >> ray.direction[2];
        root.FindIntersectedVoxels(ray, "intersected_voxels.txt");
        std::cout << "Coordinates of intersected voxels center are writed to the file intersected_voxels.txt" << std::endl;
      }
      else if (repeat == "n") {
        return 0;
      }
      else
        continue;
    }
    while (repeat != "n");

    return 0;
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}