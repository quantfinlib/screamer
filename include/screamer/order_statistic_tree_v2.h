#ifndef ORDER_STATISTIC_TREE_H
#define ORDER_STATISTIC_TREE_H

#include <memory>
#include <stdexcept>

namespace screamer {

    struct OSTNode {
        double key;
        int height;
        int size;
        std::unique_ptr<OSTNode> left;
        std::unique_ptr<OSTNode> right;

        OSTNode(double k)
            : key(k), height(1), size(1), left(nullptr), right(nullptr) {}
    };

    class OrderStatisticTree {
    public:
        OrderStatisticTree() : root(nullptr) {}

        void insert(double key) {
            root = insert(std::move(root), key);
        }

        void erase(double key) {
            root = erase(std::move(root), key);
        }

        double kth_element(int k) const {
            if (!root || k < 0 || k >= root->size) {
                throw std::out_of_range("k is out of bounds");
            }
            return kth_element(root.get(), k);
        }

        int size() const {
            return get_size(root.get());
        }

        void clear() {
            root.reset();
        }

    private:
        std::unique_ptr<OSTNode> root;

        int get_height(OSTNode* node) const {
            return node ? node->height : 0;
        }

        int get_size(OSTNode* node) const {
            return node ? node->size : 0;
        }

        void update(OSTNode* node) {
            if (node) {
                node->height = 1 + std::max(get_height(node->left.get()), get_height(node->right.get()));
                node->size = 1 + get_size(node->left.get()) + get_size(node->right.get());
            }
        }

        int get_balance(OSTNode* node) const {
            return node ? get_height(node->left.get()) - get_height(node->right.get()) : 0;
        }

        std::unique_ptr<OSTNode> rotate_right(std::unique_ptr<OSTNode> y) {
            std::unique_ptr<OSTNode> x = std::move(y->left);
            y->left = std::move(x->right);
            x->right = std::move(y);
            update(x->right.get());
            update(x.get());
            return x;
        }

        std::unique_ptr<OSTNode> rotate_left(std::unique_ptr<OSTNode> x) {
            std::unique_ptr<OSTNode> y = std::move(x->right);
            x->right = std::move(y->left);
            y->left = std::move(x);
            update(y->left.get());
            update(y.get());
            return y;
        }

        std::unique_ptr<OSTNode> insert(std::unique_ptr<OSTNode> node, double key) {
            if (!node) {
                return std::make_unique<OSTNode>(key);
            }
            if (key < node->key) {
                node->left = insert(std::move(node->left), key);
            } else {
                node->right = insert(std::move(node->right), key);
            }

            update(node.get());
            return balance(std::move(node));
        }

        std::unique_ptr<OSTNode> erase(std::unique_ptr<OSTNode> node, double key) {
            if (!node) {
                return node;
            }
            if (key < node->key) {
                node->left = erase(std::move(node->left), key);
            } else if (key > node->key) {
                node->right = erase(std::move(node->right), key);
            } else {
                if (!node->left) {
                    return std::move(node->right);
                } else if (!node->right) {
                    return std::move(node->left);
                } else {
                    OSTNode* min_larger_node = find_min(node->right.get());
                    node->key = min_larger_node->key;
                    node->right = erase(std::move(node->right), min_larger_node->key);
                }
            }
            update(node.get());
            return balance(std::move(node));
        }

        OSTNode* find_min(OSTNode* node) const {
            while (node->left) {
                node = node->left.get();
            }
            return node;
        }

        std::unique_ptr<OSTNode> balance(std::unique_ptr<OSTNode> node) {
            int balance_factor = get_balance(node.get());

            if (balance_factor > 1) {
                if (get_balance(node->left.get()) < 0) {
                    node->left = rotate_left(std::move(node->left));
                }
                return rotate_right(std::move(node));
            }

            if (balance_factor < -1) {
                if (get_balance(node->right.get()) > 0) {
                    node->right = rotate_right(std::move(node->right));
                }
                return rotate_left(std::move(node));
            }

            return node;
        }

        double kth_element(OSTNode* node, int k) const {
            int left_size = get_size(node->left.get());
            if (k < left_size) {
                return kth_element(node->left.get(), k);
            } else if (k > left_size) {
                return kth_element(node->right.get(), k - left_size - 1);
            } else {
                return node->key;
            }
        }
    };

} // namespace screamer

#endif // ORDER_STATISTIC_TREE_H
