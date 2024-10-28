#ifndef ORDER_STATISTIC_TREE_H
#define ORDER_STATISTIC_TREE_H

#include <random>
#include <memory>

namespace screamer {

    struct OSTNode {
        double key;
        int priority;
        int size;
        std::shared_ptr<OSTNode> left;
        std::shared_ptr<OSTNode> right;

        OSTNode(double k, int p)
            : key(k), priority(p), size(1), left(nullptr), right(nullptr) {}
    };

    class OrderStatisticTree {
    public:
        OrderStatisticTree()
            : root(nullptr) {}

        void insert(double key) {
            int priority = generate_priority();
            root = insert(root, std::make_shared<OSTNode>(key, priority));
        }

        void erase(double key) {
            root = erase(root, key);
        }

        double kth_element(int k) const {
            return kth_element(root, k);
        }

        int size() const {
            return get_size(root);
        }

        void clear() {
            root = nullptr;
        }

    private:
        std::shared_ptr<OSTNode> root;

        // Static function to generate random priorities
        static int generate_priority() {
            // Use thread_local to ensure thread safety
            static thread_local std::mt19937 gen(std::random_device{}());
            static thread_local std::uniform_int_distribution<> distribution;
            return distribution(gen);
        }

        int get_size(const std::shared_ptr<OSTNode>& node) const {
            return node ? node->size : 0;
        }

        void update_size(std::shared_ptr<OSTNode>& node) {
            if (node) {
                node->size = 1 + get_size(node->left) + get_size(node->right);
            }
        }

        std::shared_ptr<OSTNode> rotate_right(std::shared_ptr<OSTNode> y) {
            auto x = y->left;
            y->left = x->right;
            x->right = y;
            update_size(y);
            update_size(x);
            return x;
        }

        std::shared_ptr<OSTNode> rotate_left(std::shared_ptr<OSTNode> x) {
            auto y = x->right;
            x->right = y->left;
            y->left = x;
            update_size(x);
            update_size(y);
            return y;
        }

        std::shared_ptr<OSTNode> insert(std::shared_ptr<OSTNode> node, std::shared_ptr<OSTNode> new_node) {
            if (!node) {
                return new_node;
            }
            if (new_node->key < node->key) {
                node->left = insert(node->left, new_node);
                update_size(node);
                if (node->left->priority < node->priority) {
                    node = rotate_right(node);
                }
            } else {
                node->right = insert(node->right, new_node);
                update_size(node);
                if (node->right->priority < node->priority) {
                    node = rotate_left(node);
                }
            }
            return node;
        }

        std::shared_ptr<OSTNode> erase(std::shared_ptr<OSTNode> node, double key) {
            if (!node) {
                return node;
            }
            if (key < node->key) {
                node->left = erase(node->left, key);
            } else if (key > node->key) {
                node->right = erase(node->right, key);
            } else {
                // Node to be deleted found
                if (!node->left) {
                    return node->right;
                } else if (!node->right) {
                    return node->left;
                } else {
                    // Node with two children
                    if (node->left->priority < node->right->priority) {
                        node = rotate_right(node);
                        node->right = erase(node->right, key);
                    } else {
                        node = rotate_left(node);
                        node->left = erase(node->left, key);
                    }
                }
            }
            update_size(node);
            return node;
        }

        double kth_element(const std::shared_ptr<OSTNode>& node, int k) const {
            if (!node) {
                throw std::out_of_range("k is out of bounds");
            }
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
