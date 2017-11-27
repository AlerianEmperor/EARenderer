//
//  Collision.hpp
//  EARenderer
//
//  Created by Pavlo Muratov on 26.11.2017.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef Collision_hpp
#define Collision_hpp

#include "Interval.hpp"
#include "AxisAlignedBox3D.hpp"
#include "Ray3D.hpp"
#include "Triangle.hpp"

namespace EARenderer {
    
    class Collision {
    private:
        static Interval GetInterval(const Triangle& triangle, const glm::vec3& axis);
        static Interval GetInterval(const AxisAlignedBox3D& aabb, const glm::vec3& axis);
        
        static bool OverlapOnAxis(const AxisAlignedBox3D& aabb, const Triangle& triangle, const glm::vec3& axis);
        
    public:
        static bool TriangleAABB(const Triangle& t, const AxisAlignedBox3D& a);
        static bool RayAABB(const Ray3D& ray, const AxisAlignedBox3D& aabb, float& distance);
        static bool RayParallelogram(const Ray3D& ray, const Parallelogram3D& parallelogram, float& distance);
    };
    
}

#endif /* Collision_hpp */
