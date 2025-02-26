//
//  SparseOctree.hpp
//  EARenderer
//
//  Created by Pavel Muratov on 2/27/18.
//  Copyright © 2018 MPO. All rights reserved.
//

#ifndef SparseOctree_hpp
#define SparseOctree_hpp

#include <unordered_map>
#include <stack>
#include <vector>
#include <functional>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

#include "AxisAlignedBox3D.hpp"
#include "Ray3D.hpp"

// Design and implementation of virtualized sparse voxel octree ray casting
// http://sam.cs.lth.se/ExjobGetFile?id=359

namespace EARenderer {

    template<typename T>
    class SparseOctree {
    public:
        using ContainmentDetector = std::function<bool(const T &object, const AxisAlignedBox3D &nodeBoundingBox)>;
        using CollisionDetector = std::function<bool(const T &object, const Ray3D &ray)>;

    private:

#pragma mark - Nested types

        using NodeIndex = uint32_t;
        using BitMask = uint8_t;

        static const NodeIndex RootNodeIndex = 0b1;

    public:

        struct Node {
        private:
            friend SparseOctree;

            // 8 MSB - child presence mask, 8 LSB - leaf mask
            uint16_t mChildInfo = 0;

            AxisAlignedBox3D mBoundingBox;
            std::vector<T> mObjects;

            void setChildPresent(BitMask childIndex, bool isPresent);

            void setChildLeaf(BitMask childIndex, bool isLeaf);

            bool isChildPresent(BitMask childIndex) const;

            bool isChildLeaf(BitMask childIndex) const;

        public:
            const AxisAlignedBox3D &boundingBox() const;

            const std::vector<T> &objects() const;
        };

    private:

        struct StackFrame {
            NodeIndex nodeIndex;
            uint8_t depth;
            float t_in;
            float t_out;

            StackFrame(NodeIndex nodeIndex, uint8_t nodeDepth);

            StackFrame(NodeIndex nodeIndex, uint8_t nodeDepth, float t_in, float t_out);
        };

        // In the original article authors use right-handed coordinate system with Z axis
        // pointing upwards. We swap Y and Z to match conventional OpenGL left-handed coordinate system
        // while preserving child indexing rules defined in the paper.
        // (Original bitmasks are: 100 for X, 010 for Y and 001 for Z)
        //
        static constexpr BitMask XBitMask = 0b100;
        static constexpr BitMask YBitMask = 0b001;
        static constexpr BitMask ZBitMask = 0b010;

        //   ORDER OF CHILDREN
        //
        //       +---+---+  Y
        //      / 4 / 0 /|  |
        //     /---/---/ |  +--X
        //    +---+---+|0|
        //    | 6 | 2 ||.+
        //    +---+---+'1|
        //    | 7 | 3 ||.+ <--Positive Z
        //    +---+---+' <--Negative Z
        //      ^--Behind: 5

#pragma mark - Private members

        size_t mDepthCap = 10;
        size_t mMaximumDepth;
        AxisAlignedBox3D mBoundingBox;
        std::unordered_map<NodeIndex, Node> mNodes;
        std::stack<StackFrame> mTraversalStack;
        std::vector<float> mCuttingPlaneOffsets;
        ContainmentDetector mContainmentDetector;
        CollisionDetector mCollisionDetector;

#pragma mark - Private functions

        glm::mat4 localNormalizedSpaceMatrix() const;

        NodeIndex appendChildIndex(NodeIndex parent, NodeIndex child);

        BitMask signMask(const glm::vec3 &valueTriple) const;

        BitMask sortMaskByMinimum(const glm::vec3 &valueTriple) const;

        BitMask sortMaskByMaximum(const glm::vec3 &valueTriple) const;

        void pushRootNode();

        void pushChildNodes(const StackFrame &currentFrame,
                const glm::vec3 &t,
                BitMask signMaskA,
                BitMask signMaskB,
                BitMask p_first,
                BitMask p_last,
                size_t planeIntersectionCounter);

#pragma mark - Public interface

    public:

        struct Iterator {
        private:
            friend SparseOctree;

            using MapIterator = typename std::unordered_map<NodeIndex, Node>::iterator;
            using VectorIterator = typename std::vector<T>::iterator;

            MapIterator mNodesIterator;
            MapIterator mNodesEndIterator;

            Iterator(MapIterator i, MapIterator endIterator);

            Iterator(MapIterator endIterator);

        public:
            Iterator &operator++();

            const Node &operator*() const;

            const Node *operator->() const;

            bool operator!=(const Iterator &other) const;
        };

#pragma mark - Lifecycle

        SparseOctree(const AxisAlignedBox3D &boundingBox,
                size_t maximumDepth,
                const ContainmentDetector &containmentDetector,
                const CollisionDetector &collisionDetector);

#pragma mark - Building

        void insert(const T &object);

#pragma mark - Traversal

        bool raymarch(const Ray3D &ray);

        bool raymarch(const glm::vec3 &p0, const glm::vec3 &p1);

#pragma mark - Iteration

        Iterator begin();

        Iterator end();

    };

    template<typename T>
    typename SparseOctree<T>::Iterator begin(const SparseOctree<T> &tree) {
        return tree.begin();
    }

    template<typename T>
    typename SparseOctree<T>::Iterator end(const SparseOctree<T> &tree) {
        return tree.end();
    }

}

#include "SparseOctreeImpl.hpp"
#include "SparseOctreeNodeImpl.hpp"
#include "SparseOctreeIteratorImpl.hpp"
#include "SparseOctreeStackFrameImpl.hpp"

#endif /* SparseOctree_hpp */
