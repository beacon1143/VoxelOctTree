#pragma once

#include <array>

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

class VoxelOctTree {
private:
  std::array<double, 3> middle;
  double length;
  unsigned int discr;
  VoxelOctTree* anc;
  VoxelOctTree* desc[8];

public:
  VoxelOctTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr);
  ~VoxelOctTree();

  VoxelOctTree() = delete;
  VoxelOctTree(const VoxelOctTree&) = delete;
  VoxelOctTree& operator = (const VoxelOctTree& other) = delete;

  VoxelOctTree* BuildTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr, const VoxelOctTree* root);
  void AddVoxel(const std::array<double, 3> point);
  VoxelOctTree* Search(const std::array<double, 3> X) const;

  unsigned int VoxelsCount() const;
  void CounterIncreaser(unsigned int& cnt) const;
};