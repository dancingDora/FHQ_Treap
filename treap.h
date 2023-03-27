#include <algorithm>
#include <cstdint>
#include <stack>
#include <cstdio>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <stack>

using namespace std;

const unsigned MAX_SIZE = 10000000;

class RandGenerator {
private:
    uint64_t seed = 0;
    constexpr static int32_t BASE = 1103515245;
    constexpr static int32_t OFFSET = 12345;
    constexpr static int32_t MOD = 1e9 + 7;

public:
    uint32_t treap_rand() {
        seed = (BASE * seed + OFFSET) % MOD;
        return seed;
    }

    void reset() {
        seed = 0;
    }
};

template<typename T>
class Treap;

template<typename T>
class TreapNode {

private:
    T val;
    TreapNode<T> *left;
    TreapNode<T> *right;
    int32_t count; /* number of repeated elements contained in current node */
    int32_t size; /* number of nodes in current subtree */
    uint32_t weight;

    friend class Treap<T>;

public:
    TreapNode() : val(T()), left(nullptr), right(nullptr), count(0), size(0), weight(0) {}

    TreapNode(T val, TreapNode *left, TreapNode *right,
              int32_t count, int32_t size, uint32_t weight) :
            val(val), left(left), right(right),
            count(count), size(size), weight(weight) {}
//private:
//    T val;
//    int left;
//    int right;
//    int32_t count; /* number of repeated elements contained in current node */
//    int32_t size; /* number of nodes in current subtree */
//    uint32_t weight;
//
//    friend class Treap<T>;
//
//public:
//    TreapNode() : val(T()), left(0), right(0), count(0), size(0), weight(0) {}
//
//    TreapNode(T val, TreapNode *left, TreapNode *right,
//              int32_t count, int32_t size, uint32_t weight) :
//            val(val), left(left), right(right),
//            count(count), size(size), weight(weight) {};

    void clear() {
        left = right = count = size = weight = 0;
        val = T();
    }

};

template<typename T>
class Treap {

private:
//    RandGenerator rand;
//    int cnt = 0;
//    TreapNode<T> node[MAX_SIZE];
//    int root;
    RandGenerator rand;

    TreapNode<T> *treap_root = nullptr;

    TreapNode<T> *treap_x = nullptr;
    TreapNode<T> *treap_y = nullptr;
    TreapNode<T> *treap_z = nullptr;
    TreapNode<T> *treap_cur = nullptr;
public:
    void update(TreapNode<T> *x) {
        int ll = (x->left) ? x->left->size : 0;
        int rr = (x->right) ? x->right->size : 0;
        x->size = ll + rr + 1;
    }

    void split(TreapNode<T> *now, T v, TreapNode<T> **x, TreapNode<T> **y) {
        if (now == nullptr) {
            *x = nullptr;
            *y = nullptr;
            return;
        } else if (now->val <= v) {
            *x = now;
            split(now->right, v, &(now->right), y);
        } else {
            *y = now;
            split(now->left, v, x, &(now->left));
        }
        update(now);
    }

    TreapNode<T> *merge(TreapNode<T> *x, TreapNode<T> *y) {
        if (x == nullptr || y == nullptr) {
            if (x == nullptr) return y;
            else return x;
        }
        if (x->weight > y->weight) {// notice
            x->right = merge(x->right, y);
            update(x);
            return x;
        } else {
            y->left = merge(x, y->left);
            update(y);
            return y;
        }
    }

    TreapNode<T> *search(TreapNode<T> *x, T v) {
        if(x == nullptr) return nullptr;
        TreapNode<T> *newNode = x;
        if(newNode->val == v) return newNode;
        else if(newNode->val < v) return search(newNode->right, v);
        else if(newNode->val > v) return search(newNode->left, v);
    };


    TreapNode<T> *newNode(T x) {
        TreapNode<T> *newNode = new TreapNode<T>(x, nullptr, nullptr, 0, 1, 0);
        newNode->weight = rand.treap_rand();
        newNode->count = 1;
        return newNode;
    }

    void insert(T val) {
        TreapNode<T> *x = nullptr;
        TreapNode<T> *y = nullptr;
        /* Your code here. */
        split(treap_root, val, &x, &y);
        TreapNode<T> *res = merge(x, newNode(val));
        treap_root = merge(res, y);
    }

    void remove(T val) {
        /* Your code here. */
//        int x, y, z;
        TreapNode<T> *x = nullptr;
        TreapNode<T> *y = nullptr;
        TreapNode<T> *z = nullptr;

        split(treap_root, val, &x, &z);
        split(x, val - 1, &x, &y);
        y = merge(y->left, y->right);
        treap_root = merge(merge(x, y), z);
    }

    T pre_element(T val) {
        /* Your code here. */
        TreapNode<T> *x = nullptr;
        TreapNode<T> *y = nullptr;
        TreapNode<T> *cur = nullptr;
        split(treap_root, val - 1, &x, &y);
        cur = x;
        while (cur->right) cur = cur->right;
        merge(x, y);
        return cur->val;
    }

    T suc_element(T val) {
        /* Your code here. */
        TreapNode<T> *x = nullptr;
        TreapNode<T> *y = nullptr;
        TreapNode<T> *cur = nullptr;
        split(treap_root, val, &x, &y);
        cur = y;
        if(cur == nullptr) {
            merge(x, y);
            return cur->val;
        }
        while (cur->left) cur = cur->left;
        treap_root = merge(x, y);
        return cur->val;
    }

    int32_t rank(T val) {
        /* Your code here. */
//        int x, y, ans;
        TreapNode<T> *x;
        TreapNode<T> *y;
        int32_t ans;
        split(treap_root, val - 1, &x, &y);
        ans = x->size + 1;
        merge(x, y);
        return ans;
    }

    int32_t kth_element(int32_t rk) {
        /* Your code here. */
        TreapNode<T> *now = treap_root;
        while (now) {
            if (now->left)
                if (now->left->size + 1 == rk)
                    break;
                else if (now->left->size >= rk)
                    now = now->left;
                else {
                    rk -= now->left->size + 1;
                    now = now->right;
                }
            else if (1 == rk) break;

        }
        return now->val;
    }

    void clear() {
        /* Your code here. */
        treap_root = nullptr;
    }

    std::string pre_traverse() {
//        return "its ok.\n";
        /* Your code here. */
        if (treap_root == nullptr) return "";
        int cnt = 0;
        stack<TreapNode<T>> ss;
        while (!ss.empty())ss.pop();
        string res = "";
        ss.push(*treap_root);
        while (!ss.empty()) {
            TreapNode<T> node = ss.top();
            ss.pop();
            res = res + to_string(node.val) + " ";
            cout << "res : " << res << " cnt = "<< ++cnt << endl;
            if (node.right != nullptr) {
                cout << "in r\n";
                ss.push(*node.right);
            }
            if (node.left != nullptr) {
                cout << "in l\n";
                ss.push(*node.left);
            }
        }
        return res;
    }
};
