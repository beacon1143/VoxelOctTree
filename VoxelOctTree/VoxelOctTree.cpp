// VoxelOctTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "VoxelOctTree.hpp"

namespace VOXEL_OCTTREE {

  VoxelOctTree::VoxelOctTree() {
    for (int i = 0; i < 3; i++) {
      middle[i] = 0.0;
    }
    length = 0.0;
    discr = 0;
    //anc = nullptr;
    for (int i = 0; i < 8; i++) {
      desc[i] = nullptr;
    }
  }

  VoxelOctTree::VoxelOctTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr) {
    for (int i = 0; i < 3; i++) {
      middle[i] = _middle[i];
    }
    length = _length;
    discr = _discr;
    //anc = nullptr;
    for (int i = 0; i < 8; i++) {
      desc[i] = nullptr;
    }
  }

  VoxelOctTree::~VoxelOctTree() {
    DeleteTree(this);
  }

  /*void VoxelOctTree::SetMiddle(const std::array<double, 3> _middle) {
    middle = _middle;
  }

  void VoxelOctTree::SetLength(const double _length) {
    length = _length;
  }

  void VoxelOctTree::SetDiscr(const unsigned int _discr) {
    discr = _discr;
  }*/

  VoxelOctTree* VoxelOctTree::BuildTree(const std::array<double, 3> _middle, const double _length, const unsigned int _discr, const VoxelOctTree* root) {
    VoxelOctTree* new_node = new VoxelOctTree(_middle, _length, _discr);
    //new_node->anc = const_cast<VoxelOctTree*>(root);
    return new_node;
  }

  void VoxelOctTree::DeleteTree(VoxelOctTree* root) {
    if (root != nullptr && root->discr > 0) {
      //std::cout << root->discr << std::endl;
      std::array<VoxelOctTree*, 8> descenants;
      for (int i = 0; i < 8; i++) {
        descenants[i] = root->desc[i];
        DeleteTree(descenants[i]);
        if (root->desc[i] != nullptr) {
          delete root->desc[i];
          root->desc[i] = nullptr;
        }
      }      
    }
  }

  bool VoxelOctTree::CreateSvoFromPointCloud(const std::string fileName, const unsigned int discretization) {
    std::array<double, 3> x;
    std::array<double, 3> x_min, x_max, x_mid;
    std::list< std::array<double, 3> > pointsList;
    //int pointsListSize = 0;

    std::fstream fil;
    fil.open(fileName.c_str(), std::fstream::in);
    if (!fil.is_open()) {
      std::cerr << "Cannot open file " << fileName.c_str() << std::endl;
      return false;
    }

    for (int i = 0; i < 3; i++) {
      x_min[i] = std::numeric_limits<double>::max();
      x_max[i] = -std::numeric_limits<double>::max();
    }
  
    while (fil >> x[0] >> x[1] >> x[2]) {
      pointsList.emplace_back(x);
      //pointsListSize++;
    
      for (int i = 0; i < 3; i++) {
        if(x[i] < x_min[i]) {
          x_min[i] = x[i];
        }
        if(x[i] > x_max[i]) {
          x_max[i] = x[i];
        }
      }
    }
    fil.close();

    /*std::cout << x_max[0] << '\t' << x_max[1] << '\t' << x_max[2] << std::endl;
    std::cout << x_min[0] << '\t' << x_min[1] << '\t' << x_min[2] << std::endl;*/

    for(int i = 0; i < 3; i++) {
      x_mid[i] = (x_max[i] + x_min[i]) / 2.0;
    }

    double model_size = std::max((x_max[0] - x_min[0]), std::max(x_max[1] - x_min[1], x_max[2] - x_min[2]));

    this->middle = x_mid;
    this->length = model_size;
    this->discr = discretization;

    for (auto& point : pointsList) {        
      this->AddVoxel(point);
    }

    /*auto it = pointsList.begin();
  #pragma omp parallel for
    for (int i = 0; i < pointsListSize; i++) {
      this->AddVoxel(*it);
      it++;
    }*/

    //std::cout << this->VoxelsCount() << std::endl;

    return true;
  }

  void VoxelOctTree::AddVoxel(const std::array<double, 3> point) {
    if (this->discr == 0) {
      return;
    }

    /*if ( (point[0] == 37.534) && (point[1] == 71.774) && (point[2] == 7.737) ) {
      getchar();
    }*/

    if ( (point[0] >= middle[0]) && (point[0] <= middle[0] + 0.5*length) ) { // Xmore
      if ( (point[1] >= middle[1]) && (point[1] <= middle[1] + 0.5*length) ) { // Ymore
        if ( (point[2] >= middle[2]) && (point[2] <= middle[2] + 0.5*length) ) { // Zmore
          if (this->desc[static_cast<int>(XmoreYmoreZmore)] == nullptr) {
            std::array<double, 3> new_middle = {this->middle[0] + 0.25*length, this->middle[1] + 0.25*length, this->middle[2] + 0.25*length};
            this->desc[static_cast<int>(XmoreYmoreZmore)] = BuildTree(new_middle, 0.5 * this->length, this->discr - 1, this);
          }
          this->desc[static_cast<int>(XmoreYmoreZmore)]->AddVoxel(point);
        }
        else if( (point[2] <= middle[2]) /*&& (point[2] >= middle[2] - length/2.0)*/ ) { // Zless
          if (this->desc[static_cast<int>(XmoreYmoreZless)] == nullptr) {
            std::array<double, 3> new_middle = {this->middle[0] + 0.25*length, this->middle[1] + 0.25*length, this->middle[2] - 0.25*length};
            this->desc[static_cast<int>(XmoreYmoreZless)] = BuildTree(new_middle, 0.5 * this->length, this->discr - 1, this);
          }
          this->desc[static_cast<int>(XmoreYmoreZless)]->AddVoxel(point);
        }
        else { // Z doesn't fit
          std::cerr << "Warning: Point " << point[0] << " " << point[1] << " " << point[2] << " is outside the OctTree!" << std::endl;
          return;
        }
      }
      else if ( (point[1] <= middle[1]) /*&& (point[1] >= middle[1] - length/2.0)*/ ) { // Yless
        if ( (point[2] >= middle[2]) && (point[2] <= middle[2] + 0.5*length) ) { // Zmore
          if (this->desc[static_cast<int>(XmoreYlessZmore)] == nullptr) {
            std::array<double, 3> new_middle = {this->middle[0] + 0.25*length, this->middle[1] - 0.25*length, this->middle[2] + 0.25*length};
            this->desc[static_cast<int>(XmoreYlessZmore)] = BuildTree(new_middle, 0.5 * this->length, this->discr - 1, this);
          }
          this->desc[static_cast<int>(XmoreYlessZmore)]->AddVoxel(point);
        }
        else if ( (point[2] <= middle[2]) /*&& (point[2] >= middle[2] - length/2.0)*/ ) { // Zless
          if (this->desc[static_cast<int>(XmoreYlessZless)] == nullptr) {
            std::array<double, 3> new_middle = {this->middle[0] + 0.25*length, this->middle[1] - 0.25*length, this->middle[2] - 0.25*length};
            this->desc[static_cast<int>(XmoreYlessZless)] = BuildTree(new_middle, 0.5 * this->length, this->discr - 1, this);
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
      if ( (point[1] >= middle[1]) && (point[1] <= middle[1] + 0.5*length) ) { // Ymore
        if ( (point[2] >= middle[2]) && (point[2] <= middle[2] + 0.5*length) ) { // Zmore
          if (this->desc[static_cast<int>(XlessYmoreZmore)] == nullptr) {
            std::array<double, 3> new_middle = {this->middle[0] - 0.25*length, this->middle[1] + 0.25*length, this->middle[2] + 0.25*length};
            this->desc[static_cast<int>(XlessYmoreZmore)] = BuildTree(new_middle, 0.5 * this->length, this->discr - 1, this);
          }
          this->desc[static_cast<int>(XlessYmoreZmore)]->AddVoxel(point);
        }
        else if ( (point[2] < middle[2]) /*&& (point[2] >= middle[2] - length/2.0)*/ ) { // Zless
          if (this->desc[static_cast<int>(XlessYmoreZless)] == nullptr) {
            std::array<double, 3> new_middle = {this->middle[0] - 0.25*length, this->middle[1] + 0.25*length, this->middle[2] - 0.25*length};
            this->desc[static_cast<int>(XlessYmoreZless)] = BuildTree(new_middle, 0.5 * this->length, this->discr - 1, this);
          }
          this->desc[static_cast<int>(XlessYmoreZless)]->AddVoxel(point);
        }
        else { // Z doesn't fit
          std::cerr << "Warning: Point " << point[0] << " " << point[1] << " " << point[2] << " is outside the OctTree!" << std::endl;
          return;
        }
      }
      else if ( (point[1] <= middle[1]) /*&& (point[1] >= middle[1] - length/2.0)*/ ) { // Yless
        if ( (point[2] >= middle[2]) && (point[2] <= middle[2] + 0.5*length) ) { // Zmore
          if (this->desc[static_cast<int>(XlessYlessZmore)] == nullptr) {
            std::array<double, 3> new_middle = {this->middle[0] - 0.25*length, this->middle[1] - 0.25*length, this->middle[2] + 0.25*length};
            this->desc[static_cast<int>(XlessYlessZmore)] = BuildTree(new_middle, 0.5 * this->length, this->discr - 1, this);
          }
          this->desc[static_cast<int>(XlessYlessZmore)]->AddVoxel(point);
        }
        else if ( (point[2] <= middle[2]) /* && (point[2] >= middle[2] - length/2.0)*/ ) { // Zless
          if (this->desc[static_cast<int>(XlessYlessZless)] == nullptr) {
            std::array<double, 3> new_middle = {this->middle[0] - 0.25*length, this->middle[1] - 0.25*length, this->middle[2] - 0.25*length};
            this->desc[static_cast<int>(XlessYlessZless)] = BuildTree(new_middle, 0.5 * this->length, this->discr - 1, this);
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

  unsigned int VoxelOctTree::VoxelsCount() const {
    unsigned int counter = 0;
    //this->CounterIncreaser(counter);  

     std::function<void(const VoxelOctTree* oct_tree)>CounterIncreaser = [&](const VoxelOctTree* oct_tree) /*mutable*/ {
      if(oct_tree->discr == 0) {
        counter++;
      }
      else {
        for (int i = 0; i < 8; i++) {
          if(oct_tree->desc[i] != nullptr) {
            CounterIncreaser(oct_tree->desc[i]);
          }
        }
      }
    };

    CounterIncreaser(this);
    return counter;
  }

  bool VoxelOctTree::IntersectRayBrick(const Ray& ray) const {
    const Brick brick = {middle[0] - 0.5 * length, middle[1] - 0.5 * length, middle[2] - 0.5 * length,
                         middle[0] + 0.5 * length, middle[1] + 0.5 * length, middle[2] + 0.5 * length};
  
    // check whether initial point is inside the parallelepiped
    if ( ray.start[0] >= brick.min_point[0] && ray.start[0] <= brick.max_point[0] &&
         ray.start[1] >= brick.min_point[1] && ray.start[1] <= brick.max_point[1] &&
         ray.start[2] >= brick.min_point[2] && ray.start[2] <= brick.max_point[2] ) {
      return true;
    }

    // ray parameter
    double t_near = std::numeric_limits<double>::min(),
           t_far  = std::numeric_limits<double>::max();
    double t1, t2;

    // directions loop
    for (int i = 0; i < 3; i++) {
      if (ray.direction[i] != 0.0) {
        t1 = (brick.min_point[i] - ray.start[i]) / ray.direction[i];
        t2 = (brick.max_point[i] - ray.start[i]) / ray.direction[i];
    
        if (t1 > t2)
          std::swap(t1, t2);
        if (t1 > t_near)
          t_near = t1;
        if (t2 < t_far)
          t_far = t2;

        if (t_near > t_far)
          return false;
        if (t_far < 0.0)
          return false;
      } // if
      else {
        if ( ray.start[i] < brick.min_point[i] || ray.start[i] > brick.max_point[i] )
          return false;
      }
    } // for

    return (t_near <= t_far && t_far >=0);
  }

  std::array<int, 8> VoxelOctTree::GetOrderArray(const Ray& ray) const {
    std::array<ancestors, 8> AncOrder;

    if (ray.direction[0] >= 0.0) { // x > 0
      if (ray.direction[1] >= 0.0) { // y > 0
        if (ray.direction[2] >= 0.0) { // z > 0
          AncOrder = {XlessYlessZless, XlessYlessZmore, XlessYmoreZless, XlessYmoreZmore, XmoreYlessZless, XmoreYlessZmore, XmoreYmoreZless, XmoreYmoreZmore};
        }
        else { // z < 0
          AncOrder = {XlessYlessZmore, XlessYlessZless, XlessYmoreZmore, XlessYmoreZless, XmoreYlessZmore, XmoreYlessZless, XmoreYmoreZmore, XmoreYmoreZless};
        }
      }
      else { // y < 0
        if (ray.direction[2] >= 0.0) { // z > 0
          AncOrder = {XlessYmoreZless, XlessYmoreZmore, XlessYlessZless, XlessYlessZmore, XmoreYmoreZless, XmoreYmoreZmore, XmoreYlessZless, XmoreYlessZmore};
        }
        else { // z < 0
          AncOrder = {XlessYmoreZmore, XlessYmoreZless, XlessYlessZmore, XlessYlessZless, XmoreYmoreZmore, XmoreYmoreZless, XmoreYlessZmore, XmoreYlessZless};
        }
      }
    }
    else { // x < 0
      if (ray.direction[1] >= 0.0) { // y > 0
        if (ray.direction[2] >= 0.0) { // z > 0
          AncOrder = {XmoreYlessZless, XmoreYlessZmore, XmoreYmoreZless, XmoreYmoreZmore, XlessYlessZless, XlessYlessZmore, XlessYmoreZless, XlessYmoreZmore};
        }
        else { // z < 0
          AncOrder = {XmoreYlessZmore, XmoreYlessZless, XmoreYmoreZmore, XmoreYmoreZless, XlessYlessZmore, XlessYlessZless, XlessYmoreZmore, XlessYmoreZless};
        }
      }
      else { // y < 0
        if(ray.direction[2] >= 0.0) { // z > 0
          AncOrder = {XmoreYmoreZless, XmoreYmoreZmore, XmoreYlessZless, XmoreYlessZmore, XlessYmoreZless, XlessYmoreZmore, XlessYlessZless, XlessYlessZmore};
        }
        else { // z < 0
          AncOrder = {XmoreYmoreZmore, XmoreYmoreZless, XmoreYlessZmore, XmoreYlessZless, XlessYmoreZmore, XlessYmoreZless, XlessYlessZmore, XlessYlessZless};
        }
      }
    }

    std::array<int, 8> _AncOrder;
    for (int i = 0; i < 8; i++) {
      _AncOrder[i] = static_cast<int>(AncOrder[i]);
    }

    return _AncOrder;
  }

  void VoxelOctTree::FindIntersectedVoxels(const Ray& ray, std::string file_name) const {

    std::fstream output_file;
    output_file.open(file_name.c_str(), std::fstream::out);
    if(!output_file.is_open()) {
      std::cerr << "Cannot open file " << file_name.c_str() << std::endl;
      return;
    }

    // sequence of ancestors along the ray
    std::array<int, 8> AncOrder = GetOrderArray(ray);  
    
    std::function<void(const VoxelOctTree* node, const Ray& ray)> IntersectNode = [&](const VoxelOctTree* node, const Ray& ray) mutable {
      if ( node->IntersectRayBrick(ray) ) {
        //std::cout << "Intersected! Size is " << node->discr << std::endl;
        if (node->discr == 0) {
          output_file << node->middle[0] << " " << node->middle[1] << " " << node->middle[2] << std::endl;
        }
        else {
          for (int i = 0; i < 8; i++) {
            if (node->desc[AncOrder[i]] != nullptr )
              IntersectNode(node->desc[AncOrder[i]], ray);
          } // for
        } // else
      } // if    
    };

    IntersectNode(this, ray);

    output_file.close();
  }

} // namespace