#include "stdafx.h"
#include "VoxelOctTree.hpp"

#include <math.h>
#include <iostream>
#include <string>


void main()
{
  VoxelOctTree root;
  std::string fileName;
  unsigned int discr;
  std::cout << "Input name of the file with point cloud:\n";
  std::cin >> fileName;
  std::cout << "Input degree of discretization (number of levels in voxel octtree:\n";
  std::cin >> discr;

  if ( root.CreateSvoFromPointCloud(fileName, discr/*, root*/) ) {
    Ray ray = {0, 75, 11, 1.0, 0.0, 0.0};
    root.FindIntersectedVoxels(ray, "intersected_voxels.txt");    
  }
  else
    return;  
}