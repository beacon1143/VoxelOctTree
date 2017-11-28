#include "stdafx.h"
#include "VoxelOctTree.hpp"

#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <limits>

void main()
{
  std::array<double, 3> x;
  std::array<double, 3> x_min, x_max, x_mid;
  std::list< std::array<double, 3> > pointsList;
  
  const std::string fileName = "transformer.xyz";
  std::fstream fil;
  fil.open(fileName.c_str(), std::fstream::in /*| std::fstream::out | std::fstream::app*/);
  if (!fil.is_open()) {
    std::cerr << "Cannot open file " << fileName.c_str() << std::endl;
    return;
  }

  for (int i = 0; i < 3; i++) {
    x_min[i] = std::numeric_limits<double>::max();
    x_max[i] = -std::numeric_limits<double>::max();
  }
  
  while (fil >> x[0] >> x[1] >> x[2]) {
    pointsList.emplace_back(x);
    
    for(int i = 0; i < 3; i++) {
      if(x[i] < x_min[i]) {
        x_min[i] = x[i];
      }
      if(x[i] > x_max[i]) {
        x_max[i] = x[i];
      }
    }
  }
  fil.close();

  std::cout << x_max[0] << '\t' << x_max[1] << '\t' << x_max[2] << std::endl;
  std::cout << x_min[0] << '\t' << x_min[1] << '\t' << x_min[2] << std::endl;

  //mid x_midle = { (x_max[0] + x_min[0])/2.0, (x_max[1] + x_min[1])/2.0, (x_max[2] + x_min[2])/2.0};

  for(int i = 0; i < 3; i++) {
    x_mid[i] = (x_max[i] + x_min[i]) / 2.0;
  }

  double model_size = std::max((x_max[0] - x_min[0]), std::max(x_max[1] - x_min[1], x_max[2] - x_min[2]));

  VoxelOctTree root(x_mid, model_size, 8);

  for (auto& point : pointsList) {
    root.AddVoxel(point);
  }

  std::cout << root.VoxelsCount() << std::endl;
  
  Ray ray = {x_mid[0], x_mid[1], x_mid[2], 1.0, 0.0, 0.0};
  root.FindIntersectedVoxels(ray, "intersected_voxels.txt");
}