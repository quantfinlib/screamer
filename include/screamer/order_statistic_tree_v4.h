#ifndef ORDER_STATISTIC_TREE_H
#define ORDER_STATISTIC_TREE_H

#include <stdexcept>
#include <vector>
#include <limits>
#include <cmath>

namespace screamer {

    class OrderStatisticTree {
    public:
        OrderStatisticTree(int max_window_size)
            : root(nullptr), pool_index(0)
        {
            // Initialize node pool with extra capacity to handle balancing
            node_pool.reserve(max_window_size * 2);
        }

        void insert(double key) {
            root = insert(root, key);
        }

        void erase(double key) {
            root = erase(root, key);
        }

        double kth_element(int k) const {
            if (!root || k < 0 || k >= root->size) {
                throw std::out_of_range("k is out of bounds");
            }
            return kth_element(root, k);
        }

        int size() const {
            return get_size(root);
        }

        void clear() {
            root = nullptr;
            pool_index = 0;
            free_list.clear();
        }

    private:
        struct OSTNode {
            double key;
            int height;
            int size;
            OSTNode* left;
            OSTNode* right;

            OSTNode() : key(0), height(1), size(1), left(nullptr), right(nullptr) {}
        };

        OSTNode* root;

        // Memory pool for nodes
        std::vector<OSTNode> node_pool;
        size_t pool_index; // Index of the next available node in the pool

        // Free list of nodes
        std::vector<OSTNode*> free_list;

        // Allocate a node from the pool or free list
        OSTNode* allocate_node(double key) {
            OSTNode* node;
            if (!free_list.empty()) {
                node = free_list.back();
                free_list.pop_back();
            } else {
                if (pool_index >= node_pool.size()) {
                    // Expand the pool dynamically
                    node_pool.emplace_back();
                }
                node = &node_pool[pool_index++];
            }
            node->key = key;
            node->height = 1;
            node->size = 1;
            node->left = nullptr;
            node->right = nullptr;
            return node;
        }

        // Return a node to the free list
        void deallocate_node(OSTNode* node) {
            free_list.push_back(node);
        }

        int get_height(OSTNode* node) const {
            return node ? node->height : 0;
        }

        int get_size(OSTNode* node) const {
            return node ? node->size : 0;
        }

        void update(OSTNode* node) {
            if (node) {
                node->height = 1 + std::max(get_height(node->left), get_height(node->right));
                node->size = 1 + get_size(node->left) + get_size(node->right);
            }
        }

        int get_balance(OSTNode* node) const {
            return node ? get_height(node->left) - get_height(node->right) : 0;
        }

        OSTNode* rotate_right(OSTNode* y) {
            OSTNode* x = y->left;
            OSTNode* T2 = x->right;

            // Perform rotation
            x->right = y;
            y->left = T2;

            // Update heights and sizes
            update(y);
            update(x);

            // Return new root
            return x;
        }

        OSTNode* rotate_left(OSTNode* x) {
            OSTNode* y = x->right;
            OSTNode* T2 = y->left;

            // Perform rotation
            y->left = x;
            x->right = T2;

            // Update heights and sizes
            update(x);
            update(y);

            // Return new root
            return y;
        }

        OSTNode* insert(OSTNode* node, double key) {
            if (!node) {
                return allocate_node(key);
            }
            if (key <= node->key) {
                node->left = insert(node->left, key);
            } else {
                node->right = insert(node->right, key);
            }

            update(node);
            return balance(node);
        }

        OSTNode* erase(OSTNode* node, double key) {
            if (!node) {
                return node;
            }

            if (key < node->key) {
                node->left = erase(node->left, key);
            } else if (key > node->key) {
                node->right = erase(node->right, key);
            } else {
                // Node with only one child or no child
                if (!node->left || !node->right) {
                    OSTNode* temp = node->left ? node->left : node->right;

                    // No child case
                    if (!temp) {
                        temp = node;
                        node = nullptr;
                    } else {
                        // One child case
                        *node = *temp;
                    }

                    deallocate_node(temp);
                } else {
                    // Node with two children: Get the inorder successor
                    OSTNode* temp = min_value_node(node->right);

                    // Copy the inorder successor's content to this node
                    node->key = temp->key;

                    // Delete the inorder successor
                    node->right = erase(node->right, temp->key);
                }
            }

            // If the tree had only one node then return
            if (!node) {
                return node;
            }

            // Update height and size
            update(node);

            // Balance the node
            return balance(node);
        }



        OSTNode* min_value_node(OSTNode* node) const {
            OSTNode* current = node;
            while (current->left) {
                current = current->left;
            }
            return current;
        }

        OSTNode* balance(OSTNode* node) {
            int balance_factor = get_balance(node);

            // Left Left Case
            if (balance_factor > 1 && get_balance(node->left) >= 0) {
                return rotate_right(node);
            }

            // Left Right Case
            if (balance_factor > 1 && get_balance(node->left) < 0) {
                node->left = rotate_left(node->left);
                return rotate_right(node);
            }

            // Right Right Case
            if (balance_factor < -1 && get_balance(node->right) <= 0) {
                return rotate_left(node);
            }

            // Right Left Case
            if (balance_factor < -1 && get_balance(node->right) > 0) {
                node->right = rotate_right(node->right);
                return rotate_left(node);
            }

            return node;
        }

        double kth_element(OSTNode* node, int k) const {
            int left_size = get_size(node->left);
            if (k < left_size) {
                return kth_element(node->left, k);
            } else if (k > left_size) {
                return kth_element(node->right, k - left_size - 1);
            } else {
                return node->key;
            }
        }
    };

} // namespace screamer

#endif // ORDER_STATISTIC_TREE_H
