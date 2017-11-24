// VoxelOctTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "VoxelOctTree.hpp"

VoxelOctTree::VoxelOctTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr) {
  for (int i = 0; i < 3; i++) {
    middle[i] = _middle[i];
  }
  length = _length;
  discr = _discr;
  anc = nullptr;
  for (int i = 0; i < 8; i++) {
    desc[i] = nullptr;
  }
}

VoxelOctTree::~VoxelOctTree() {
}

VoxelOctTree* VoxelOctTree::BuildTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr, const VoxelOctTree* root) {
  VoxelOctTree* new_node = new VoxelOctTree(_middle, _length, _discr);
  new_node->anc = const_cast<VoxelOctTree*>(root);
  return new_node;
}

void VoxelOctTree::AddVoxel(const std::array<double, 3> point) {
  if (this->discr == 0) {
    return;
  }

  /*if ( (point[0] == 37.534) && (point[1] == 71.774) && (point[2] == 7.737) ) {
    getchar();
  }*/

  if ( (point[0] >= middle[0]) && (point[0] <= middle[0] + length/2.0) ) { // Xmore
    if ( (point[1] >= middle[1]) && (point[1] <= middle[1] + length/2.0) ) { // Ymore
      if ( (point[2] >= middle[2]) && (point[2] <= middle[2] + length/2.0) ) { // Zmore
        if (this->desc[static_cast<int>(XmoreYmoreZmore)] == nullptr) {
          std::array<double, 3> new_middle = {this->middle[0] + length/4.0, this->middle[1] + length/4.0, this->middle[2] + length/4.0};
          this->desc[static_cast<int>(XmoreYmoreZmore)] = BuildTree(new_middle, this->length / 2.0, this->discr - 1, this);
        }
        this->desc[static_cast<int>(XmoreYmoreZmore)]->AddVoxel(point);
      }
      else if( (point[2] <= middle[2]) /*&& (point[2] >= middle[2] - length/2.0)*/ ) { // Zless
        if (this->desc[static_cast<int>(XmoreYmoreZless)] == nullptr) {
          std::array<double, 3> new_middle = {this->middle[0] + length/4.0, this->middle[1] + length/4.0, this->middle[2] - length/4.0};
          this->desc[static_cast<int>(XmoreYmoreZless)] = BuildTree(new_middle, this->length / 2.0, this->discr - 1, this);
        }
        this->desc[static_cast<int>(XmoreYmoreZless)]->AddVoxel(point);
      }
      else { // Z doesn't fit
        std::cerr << "Warning: Point " << point[0] << " " << point[1] << " " << point[2] << " is outside the OctTree!" << std::endl;
        return;
      }
    }
    else if ( (point[1] <= middle[1]) /*&& (point[1] >= middle[1] - length/2.0)*/ ) { // Yless
      if ( (point[2] >= middle[2]) && (point[2] <= middle[2] + length/2.0) ) { // Zmore
        if (this->desc[static_cast<int>(XmoreYlessZmore)] == nullptr) {
          std::array<double, 3> new_middle = {this->middle[0] + length/4.0, this->middle[1] - length/4.0, this->middle[2] + length/4.0};
          this->desc[static_cast<int>(XmoreYlessZmore)] = BuildTree(new_middle, this->length / 2.0, this->discr - 1, this);
        }
        this->desc[static_cast<int>(XmoreYlessZmore)]->AddVoxel(point);
      }
      else if ( (point[2] <= middle[2]) /*&& (point[2] >= middle[2] - length/2.0)*/ ) { // Zless
        if (this->desc[static_cast<int>(XmoreYlessZless)] == nullptr) {
          std::array<double, 3> new_middle = {this->middle[0] + length/4.0, this->middle[1] - length/4.0, this->middle[2] - length/4.0};
          this->desc[static_cast<int>(XmoreYlessZless)] = BuildTree(new_middle, this->length / 2.0, this->discr - 1, this);
        }
        this->desc[static_cast<int>(XmoreYlessZless)]->AddVoxel(point);
      }
      else { // Z doesn't fit
        std::cerr << "Warning: Point " << point[0] << " " << point[1] << " " << point[2] << " is outside the OctTree!" << std::endl;
        return;
      }
    }
    else { // Y doesn't fit
      std::cerr << "Warning: Point " << point[0] << " " << point[1] << " " << point[2] << " is outside the OctTree!" << std::endl;
      return;
    }
  }

  else if ( (point[0] <= middle[0]) /*&& (point[0] >= middle[0] - length/2.0)*/ ) { // Xless
    if ( (point[1] >= middle[1]) && (point[1] <= middle[1] + length/2.0) ) { // Ymore
      if ( (point[2] >= middle[2]) && (point[2] <= middle[2] + length/2.0) ) { // Zmore
        if (this->desc[static_cast<int>(XlessYmoreZmore)] == nullptr) {
          std::array<double, 3> new_middle = {this->middle[0] - length/4.0, this->middle[1] + length/4.0, this->middle[2] + length/4.0};
          this->desc[static_cast<int>(XlessYmoreZmore)] = BuildTree(new_middle, this->length / 2.0, this->discr - 1, this);
        }
        this->desc[static_cast<int>(XlessYmoreZmore)]->AddVoxel(point);
      }
      else if ( (point[2] < middle[2]) /*&& (point[2] >= middle[2] - length/2.0)*/ ) { // Zless
        if (this->desc[static_cast<int>(XlessYmoreZless)] == nullptr) {
          std::array<double, 3> new_middle = {this->middle[0] - length/4.0, this->middle[1] + length/4.0, this->middle[2] - length/4.0};
          this->desc[static_cast<int>(XlessYmoreZless)] = BuildTree(new_middle, this->length / 2.0, this->discr - 1, this);
        }
        this->desc[static_cast<int>(XlessYmoreZless)]->AddVoxel(point);
      }
      else { // Z doesn't fit
        std::cerr << "Warning: Point " << point[0] << " " << point[1] << " " << point[2] << " is outside the OctTree!" << std::endl;
        return;
      }
    }
    else if ( (point[1] <= middle[1]) /*&& (point[1] >= middle[1] - length/2.0)*/ ) { // Yless
      if ( (point[2] >= middle[2]) && (point[2] <= middle[2] + length/2.0) ) { // Zmore
        if (this->desc[static_cast<int>(XlessYlessZmore)] == nullptr) {
          std::array<double, 3> new_middle = {this->middle[0] - length/4.0, this->middle[1] - length/4.0, this->middle[2] + length/4.0};
          this->desc[static_cast<int>(XlessYlessZmore)] = BuildTree(new_middle, this->length / 2.0, this->discr - 1, this);
        }
        this->desc[static_cast<int>(XlessYlessZmore)]->AddVoxel(point);
      }
      else if ( (point[2] <= middle[2]) /* && (point[2] >= middle[2] - length/2.0)*/ ) { // Zless
        if (this->desc[static_cast<int>(XlessYlessZless)] == nullptr) {
          std::array<double, 3> new_middle = {this->middle[0] - length/4.0, this->middle[1] - length/4.0, this->middle[2] - length/4.0};
          this->desc[static_cast<int>(XlessYlessZless)] = BuildTree(new_middle, this->length / 2.0, this->discr - 1, this);
        }
        this->desc[static_cast<int>(XlessYlessZless)]->AddVoxel(point);
      }
      else { // Z doesn't fit
        std::cerr << "Warning: Point " << point[0] << " " << point[1] << " " << point[2] << " is outside the OctTree!" << std::endl;
        return;
      }
    }
    else { // Y doesn't fit
      std::cerr << "Warning: Point " << point[0] << " " << point[1] << " " << point[2] << " is outside the OctTree!" << std::endl;
      return;
    }
  }
  else { // X doesn't fit
    std::cerr << "Warning: Point " << point[0] << " " << point[1] << " " << point[2] << " is outside the OctTree!" << std::endl;
    return;
  }
}

VoxelOctTree* VoxelOctTree::Search(const std::array<double, 3> X) const {
  return nullptr;
}

unsigned int VoxelOctTree::VoxelsCount() const {
  unsigned int counter = 0;
  this->CounterIncreaser(counter);  
  return counter;
}

void VoxelOctTree::CounterIncreaser(unsigned int& cnt) const {
  if (this->discr == 0) {
    cnt = cnt + 1;
  }
  else {
    for(int i = 0; i < 8; i++) {
      if(this->desc[i] != nullptr) {
        this->desc[i]->CounterIncreaser(cnt);
      }
    }
  }
}