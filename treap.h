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

//const unsigned MAX_SIZE = 10000000;

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

    int size = 0;
public:
    void update(TreapNode<T> *x) {
        int ll = (x->left) ? x->left->size : 0;
        int rr = (x->right) ? x->right->size : 0;
        x->size = ll + rr + x->count;
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
        if (x == nullptr) return nullptr;
        TreapNode<T> *newNode = x;
        if (newNode->val == v) return newNode;
        else if (newNode->val < v) return search(newNode->right, v);
        else if (newNode->val > v) return search(newNode->left, v);
    };

    bool searchIns(T v) {
        TreapNode<T> *res = search(treap_root, v);
        if (res == nullptr) return false;
        else {
            res->count++;
            res->size++;
            size++;
        }
    }

    bool searchDel(T v) {
        if (treap_root == nullptr) return true;
        TreapNode<T> *res = search(treap_root, v);
        if (res == nullptr || res->count == 1) return false;
        else {
            res->count--;
            res->size--;
            size--;
            return true;
        }
    }

    bool find(T v) {
        TreapNode<T> *res = search(treap_root, v);
        return (res != nullptr);
    }

//    T MIN() {
//        if (treap_root == nullptr) return -1;
//        TreapNode<T> *res = treap_root->right;
//        if (res == nullptr) return treap_root->val;
//        while (res->right) res = res->right;
//        return res->val;
//    }

//    T MAX() {
//        if (treap_root == nullptr) return -1;
//        TreapNode<T> *res = treap_root->left;
//        if (res == nullptr) return treap_root->val;
//        while (res->left) res = res->left;
//        return res->val;
//    }

    TreapNode<T> *newNode(T x) {
        auto *node = new TreapNode<T>(x, nullptr, nullptr, 0, 1, 0);
        node->weight = rand.treap_rand();
        node->count = 1;
        return node;
    }

    void insert(T val) {
        if (searchIns(val)) return;
        TreapNode<T> *x = nullptr;
        TreapNode<T> *y = nullptr;
        /* Your code here. */
        split(treap_root, val, &x, &y);
        TreapNode<T> *res = merge(x, newNode(val));
        treap_root = merge(res, y);
        size++;
    }

    void remove(T val) {
        /* Your code here. */
//        int x, y, z;
        if (!find(val)) return;
        if (!find(val)) return;
        if (searchDel(val)) return;
        TreapNode<T> *x = nullptr;
        TreapNode<T> *y = nullptr;
        TreapNode<T> *z = nullptr;

        split(treap_root, val, &x, &z);
        split(x, val - 1, &x, &y);
        y = merge(y->left, y->right);
        treap_root = merge(merge(x, y), z);
        size--;
    }

    T pre_element(T val) {
        /* Your code here. */
//        if(!find(val)) return -1;
//        if(val <= MIN()) return -1;
        TreapNode<T> *x = nullptr;
        TreapNode<T> *y = nullptr;
        split(treap_root, val - 1, &x, &y);
        TreapNode<T> *cur = x;
        if (cur == nullptr) return -1;
        while (cur->right) cur = cur->right;
        merge(x, y);
        return cur->val;
    }

    T suc_element(T val) {
//        if(val >= MAX()) return -1;
        /* Your code here. */
        TreapNode<T> *x = nullptr;
        TreapNode<T> *y = nullptr;
        split(treap_root, val, &x, &y);
        TreapNode<T> *cur = y;
        if (cur == nullptr) {
            merge(x, y);
            return -1;
        }
        while (cur->left) cur = cur->left;
        treap_root = merge(x, y);
        return cur->val;
    }

    int32_t rank(T val) {
        /* Your code here. */
//        int x, y, ans;
        if (!find(val)) return -1;
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
        if (rk > size) return -1;
        TreapNode<T> *now = treap_root;
        while (now) {
            {//看看可不可以直接找到这个数
                if (now->left) {
                    if (now->left->size + now->count <= rk && now->left->size + 1 >= rk)//修改，因为只要在区间里就返回
                        break;
                } else {
                    if (1 <= rk && rk <= now->count)
                        break;
                }
            }
            //到了这里说明没找到，就要跳
            if (now->left && now->left->size >= rk) //左边能跳就往左跳
                now = now->left;
            else {//左边跳不了又不是当前节点，那就无脑往右跳
                if (now->left)
                    rk -= now->left->size + now->count;
                else
                    rk -= now->count;
                now = now->right;
            }
//            if (1 == rk) break;
        }
        return now->val;
    }

    void destroyTree(TreapNode<T> *node) {
        if (node == nullptr) {
            return;
        }
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    void clear() {
        /* Your code here. */
//        destroyTree(treap_root);
        destroyTree(treap_root);
        treap_root->left = NULL;
        treap_root->right = NULL;
        treap_root = NULL;
        size = 0;
        rand.reset();
    }

    std::string pre_traverse() {
//        return "its ok.\n";
        /* Your code here. */
        if (treap_root == nullptr) return "empty";
//        int cnt = 0;
        stack<TreapNode<T>> ss;
        while (!ss.empty())ss.pop();
        string res;
        ss.push(*treap_root);
        while (!ss.empty()) {
            TreapNode<T> node = ss.top();
            ss.pop();
            res = res + to_string(node.val) + " ";
//            cout << "res : " << res << " cnt = "<< ++cnt << endl;
            if (node.right != nullptr) {
//                cout << "in r\n";
                ss.push(*node.right);
            }
            if (node.left != nullptr) {
//                cout << "in l\n";
                ss.push(*node.left);
            }
        }
        return res;
    }
};
