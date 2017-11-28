#pragma once

#include <array>
#include <fstream>

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
  VoxelOctTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr);
  ~VoxelOctTree();

  // default constructors and operator=
  VoxelOctTree() = delete;
  VoxelOctTree(const VoxelOctTree&) = delete;
  VoxelOctTree& operator = (const VoxelOctTree& other) = delete;

  VoxelOctTree* BuildTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr, const VoxelOctTree* root);
  static void DeleteTree(VoxelOctTree* root);

  void AddVoxel(const std::array<double, 3> point);
  VoxelOctTree* Search(const std::array<double, 3> X) const;

  unsigned int VoxelsCount() const;
  //void CounterIncreaser(unsigned int& cnt) const;

  // voxels intersected by ray
  bool IntersectRayBrick(/*const VoxelOctTree* node, */const Ray& ray) const;
  void FindIntersectedVoxels(const Ray& ray, std::string file_name) const;
};