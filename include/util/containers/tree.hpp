#pragma once 

#include <vector>

namespace util {
    namespace container {
        namespace tree {
            template <typename DataType>
            struct node {
                node<DataType>* parent;
                DataType* data;
                std::vector<node<DataType>*> children;
            };

            template <typename DataType>
            using root = node<DataType>;
        }
    }
}