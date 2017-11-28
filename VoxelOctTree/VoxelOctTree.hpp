#pragma once

#include <omp.h>
#include <array>
#include <fstream>
#include <iostream>
#include <functional>
#include <list>

typedef enum {
  XmoreYmoreZmore = 0,
  XmoreYmoreZless = 1,
  XmoreYlessZmore = 2,
  XmoreYlessZless = 3,
  XlessYmoreZmore = 4,
  XlessYmoreZless = 5,
  XlessYlessZmore = 6,
  XlessYlessZless = 7
} ancestors;

struct Brick {
  std::array<double, 3> min_point;
  std::array<double, 3> max_point;
};

struct Ray {
  std::array<double, 3> point;
  std::array<double, 3> direction;
};

class VoxelOctTree {
private:
  std::array<double, 3> middle;
  double length;         // physical size of OctTree
  unsigned int discr;    // degree of 2, i.e. amount of OctTree levels
  //VoxelOctTree* anc;
  VoxelOctTree* desc[8];

public:
  VoxelOctTree();
  VoxelOctTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr);
  ~VoxelOctTree();

  void SetMiddle(const std::array<double, 3> _middle);
  void SetLength(const double _length);
  void SetDiscr(const unsigned int _discr);

  // default constructors and operator=
  VoxelOctTree(const VoxelOctTree&) = delete;
  VoxelOctTree& operator = (const VoxelOctTree& other) = delete;

  // build and delete
  VoxelOctTree* BuildTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr, const VoxelOctTree* root);
  void DeleteTree(VoxelOctTree* root);

  bool CreateSvoFromPointCloud(const std::string fileName, const unsigned int discretization/*, VoxelOctTree& oct_tree*/);

  void AddVoxel(const std::array<double, 3> point);  
  unsigned int VoxelsCount() const;
  
  // voxels intersected by ray
  bool IntersectRayBrick(/*const VoxelOctTree* node, */const Ray& ray) const;
  void MakeOrderArray(const Ray& ray, std::array<ancestors, 8>& AncOrder) const;
  void FindIntersectedVoxels(const Ray& ray, std::string file_name) const;
};