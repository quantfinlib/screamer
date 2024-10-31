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
            : root(nullptr), pool_index(0), max_window_size(max_window_size)
        {
            // Initialize node pool with extra capacity to handle balancing
            node_pool.reserve(max_window_size * 2);
        }

        void insert(double key) {
            std::cout << "OrderStatisticTree::insert(" << key << ")" << std::endl;
            root = insert(root, key);
        }

        void erase(double key) {
            std::cout << "OrderStatisticTree::erase(" << key << ")" << std::endl;
            root = erase(root, key);
        }

        double kth_element(int k) const {
            std::cout << "OrderStatisticTree::kth_element(" << k << ")" << std::endl;

            if (!root || k < 0 || k >= root->size) {
                throw std::out_of_range("k is out of bounds");
            }
            return kth_element(root, k);
        }

        int size() const {
            return get_size(root);
        }

        void clear() {
            std::cout << "OrderStatisticTree::clear()" << std::endl;
            root = nullptr;
            pool_index = 0;
            free_list.clear();
            node_pool.clear(); // Ensure all nodes are cleared
            node_pool.reserve(max_window_size * 2); // Re-reserve to ensure pool capacity
        }        

    private:
        struct OSTNode {
            double key;
            int count; // Number of times this key appears
            int height;
            int size;
            OSTNode* left;
            OSTNode* right;
            OSTNode() : key(0), count(1), height(1), size(1), left(nullptr), right(nullptr) {}
        };

        OSTNode* root;
        int max_window_size;

        // Memory pool for nodes
        std::vector<OSTNode> node_pool;
        size_t pool_index; // Index of the next available node in the pool

        // Free list of nodes
        std::vector<OSTNode*> free_list;

        OSTNode* allocate_node(double key) {
            std::cout << "allocate_node(" << key << ")" << std::endl;
            OSTNode* node;
            if (!free_list.empty()) {
                std::cout << "- free_list is not empty" << std::endl;
                node = free_list.back();
                free_list.pop_back();
            } else {
                std::cout << "- free_list is empty" << std::endl;
                if (pool_index >= node_pool.size()) {
                    std::cout << "- Expand the pool dynamically" << std::endl;
                    // Expand the pool dynamically
                    node_pool.emplace_back(); // No other constructor parameters needed, assuming default.
                }
                node = &node_pool[pool_index++];
            }
            node->key = key;
            node->count = 1;
            node->height = 1;
            node->size = 1;
            node->left = nullptr;
            node->right = nullptr;
            return node;
        }        

        void deallocate_node(OSTNode* node) {
            std::cout << "deallocate_node(" << node->key << ")" << std::endl;
            // Before adding to free list, check if it's already deallocated
            if (std::find(free_list.begin(), free_list.end(), node) == free_list.end()) {
                free_list.push_back(node);
                std::cout << "- std::find, Deallocated node with key " << node->key << std::endl;
            }
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
                node->size = node->count + get_size(node->left) + get_size(node->right);
            }
            // Debug: Print updated node values
            std::cout << "Updated node with key " << node->key << ": size = " << node->size << ", height = " << node->height << std::endl;
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
            std::cout << "insert(" << node << "," << key << std::endl;
            if (!node) {
                std::cout << "- (!node), returning allocate_node(" << key << ")" << std::endl;
                return allocate_node(key);
            }

            if (key < node->key) {
                std::cout << "- node->left = insert(" << node->left << "," << key << ")" << std::endl;
                node->left = insert(node->left, key);
            } else if (key > node->key) {
                std::cout << "- node->right = insert(" << node->left << "," << key << ")" << std::endl;
                node->right = insert(node->right, key);
            } else {
                // Key already exists, increment count
                std::cout << "Key already exists, increment count" << std::endl;
                node->count += 1;
            }

            update(node);
            return balance(node);
        }


        OSTNode* erase(OSTNode* node, double key) {
            std::cout << "OSTNode::erase(" << node << "," << key << ")" << std::endl;
            if (!node) {
                std::cout << "- !node, return node" << node << std::endl;
                return node;
            }

            if (key < node->key) {
                std::cout << "- left erase key=" << key << std::endl;
                node->left = erase(node->left, key);
            } else if (key > node->key) {
                std::cout << "- right erase key=" << key << std::endl;
                node->right = erase(node->right, key);
            } else {
                std::cout << "- not left nor right erase" << key << std::endl;

                if (node->count > 1) {
                    std::cout << "- count=" << node->count << " > 1 ==> decrement" << std::endl;
                    // Only decrement count if the node exists more than once
                    node->count -= 1;
                    node->size -= 1;
                } else {
                    // Only deallocate when absolutely necessary
                    std::cout << "- count == 1" << std::endl;
                    if (!node->left || !node->right) {
                        std::cout << "- left || right is empty" << std::endl;
                        OSTNode* temp = node->left ? node->left : node->right;
                        deallocate_node(node);
                        node = temp;
                    } else {
                        std::cout << "- we have both left and right" << std::endl;

                        OSTNode* temp = min_value_node(node->right);
                        node->key = temp->key;
                        node->count = temp->count;
                        temp->count = 1;  // Set count to 1 for deletion
                        node->right = erase(node->right, temp->key);
                    }
                }
            }

            if (!node) return node;

            // Update and balance after deletion
            update(node);
            return balance(node);
        }



        OSTNode* min_value_node(OSTNode* node) const {
            std::cout << "min_value_node(" << node << ")" << std::endl;
            OSTNode* current = node;
            while (current->left) {
                current = current->left;
            }
            return current;
        }

        OSTNode* balance(OSTNode* node) {
            std::cout << "balance(" << node << ")" << std::endl;
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
            std::cout << "kth_element(" << node << "," << k << ")" << std::endl;
            int left_size = get_size(node->left);
            if (k < left_size) {
                return kth_element(node->left, k);
            } else if (k < left_size + node->count) {
                return node->key;
            } else {
                return kth_element(node->right, k - left_size - node->count);
            }
        }



    };

} // namespace screamer

#endif // ORDER_STATISTIC_TREE_H
