#pragma once 

#include "config.hpp"

#include <vector>

namespace util {
    namespace container {
        template <typename DataType>
        struct TreeNode {
            TreeNode<DataType>* parent;
            DataType data;
            std::vector<TreeNode<DataType>*> children;
        
            TreeNode() {
                parent = nullptr;
            };

            TreeNode(DataType data) {
                parent = nullptr;
                this->data = data;
            }

            TreeNode<DataType>* emplaceChild(DataType data) {
                TreeNode<DataType>* newNode = new TreeNode(data);
                newNode->parent = this;
                this->children.push_back(newNode);

                return newNode;
            }

            TreeNode<DataType>* emplaceChild() {
                TreeNode<DataType>* newNode = new TreeNode();
                newNode->parent = this;
                this->children.push_back(newNode);

                return newNode;
            }

            /// @brief Causes a memory leak if used in non-leaf nodes. Also shouldn't be used on nodes not created with emplaceChild() i.e. possibly the root node.
            void deleteLeaf() {
                if(this->parent) {
                    TreeNode<DataType>* parent = this->parent;
                    for(int i = 0; i < parent->children.size(); i++) {
                        if(parent->children[i] == this) {
                            parent->children.erase(i);
                        }
                    }

                    delete this;
                }
            }
        };

        template <typename DataType>
        using Tree = TreeNode<DataType>;
    }
}