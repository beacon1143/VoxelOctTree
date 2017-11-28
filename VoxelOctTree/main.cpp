#include "stdafx.h"
#include "VoxelOctTree.hpp"

#include <math.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <limits>

void main()
{
  VoxelOctTree root;
  if ( root.CreateSvoFromPointCloud("transformer.xyz", 8/*, root*/) ) {
    Ray ray = {0, 75, 11, 1.0, 0.0, 0.0};
    root.FindIntersectedVoxels(ray, "intersected_voxels.txt");
    return;
  }
  else
    return;  
}