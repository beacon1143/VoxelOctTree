#include "stdafx.h"
#include "VoxelOctTree.hpp"

#include <math.h>
#include <iostream>
#include <string>

using namespace VOXEL_OCTTREE;

int main()
{
  VoxelOctTree root;
  std::string fileName;
  unsigned int discr;
  std::cout << "Input name of the file with point cloud:\n";    // transformer.xyz
  std::cin >> fileName;
  std::cout << "Input degree of discretization (number of levels in voxel octtree):\n";    // 8
  std::cin >> discr;
  std::cout << "Creating voxel octtree, please wait..." << std::endl;

  std::string repeat;
  Ray ray;

  if ( root.CreateSvoFromPointCloud(fileName, discr) ) {
    std::cout << "Voxel octtree created" << std::endl;
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
  }
  else
    return 1;  
}