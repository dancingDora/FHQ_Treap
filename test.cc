#include <ios>
#include <iostream>
#include <cstdio>
#include <cstdint>
#include <string>
#include <fstream>
#include "treap.h"
#include "util.h"

const int TEST_NUM = 4;
uint64_t start, en, res = 0;

void generate_output(Treap<int32_t> &treap, int32_t round) {
    int32_t n, op, val;

    JudgeFile input_file(input_path(round), std::ios_base::in);
    auto &fin = input_file.get();

    JudgeFile output_file(output_path(round), std::ios_base::out);
    auto &fout = output_file.get();

    fin >> n;

    for (int i = 1; i <= n; i++) {
        fin >> op >> val;
        switch (op) {
            case 1:
                start = rdtsc();
                treap.insert(val);
                en = rdtsc();
                res += en - start;
                fout << treap.pre_traverse() << std::endl;
                break;
            case 2:
                start = rdtsc();
                treap.remove(val);
                en = rdtsc();
                res += en - start;
                fout << treap.pre_traverse() << std::endl;
                break;
            case 3:
                start = rdtsc();
                treap.rank(val);
                en = rdtsc();
                fout << treap.rank(val) << std::endl;
                res += en - start;
                break;
            case 4:
                start = rdtsc();
                treap.kth_element(val);
                en = rdtsc();

                fout << treap.kth_element(val) << std::endl;
                res += en - start;
                break;
            case 5:
                start = rdtsc();
                treap.pre_element(val);
                en = rdtsc();
                fout << treap.pre_element(val) << std::endl;
                res += en - start;
                break;
            case 6:
                start = rdtsc();
                treap.suc_element(val);
                en = rdtsc();

                fout << treap.suc_element(val) << std::endl;
                res += en - start;
                break;
        }
    }
}

bool check_answer(int32_t round) {
    JudgeFile output_file(output_path(round), std::ios_base::in);
    JudgeFile answer_file(answer_path(round), std::ios_base::in);
    auto &out_handle = output_file.get();
    auto &ans_handle = answer_file.get();
    std::string ol, al;
    bool ret;

    for (int32_t lineno = 0; !out_handle.eof() && !ans_handle.eof(); lineno++) {
        getline(out_handle, ol);
        getline(ans_handle, al);
        if (trim(ol) != trim(al)) {
            std::cerr << "ERROR: Failed at case #" << round << " line:" << lineno <<
                      " output:" << ol << " answer:" << al << std::endl;
            return false;
        }
    }
    if (!(ret = out_handle.eof() && ans_handle.eof())) {
        std::cerr << "ERROR: Line number miss match" << std::endl;
    }
    return ret;
}

int main() {
    Treap<int32_t> treap;
    std::cout << "TEST_NUM:" << TEST_NUM << std::endl;
    for (int32_t round = 1; round <= TEST_NUM; round++) {
        try {
            generate_output(treap, round);
//            cout << treap.height() << endl;
            cout << '\n';
            cout << "split times is " << treap.splitNum << '\n';
            cout << "split cost time is " << treap.splitTime << '\n';
            cout << "merge times is " << treap.mergeNum << '\n';
            cout << "merge cost time is " << treap.mergeTime << '\n' << '\n';
            cout << "round " << round << " total time cost : " << res;
            cout << '\n';

            treap.clear();
            res = 0;
            if (!check_answer(round))
                return -1;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return -1;
        }
    }

//    string filename = "./analyse/test";
//
//    for(int i = 1; i <= 20; i++) {
//        filename = "./analyse/test";
//        filename += to_string(i);
//        int n = 500 * i;
//        int a,b;
//        ifstream fin(filename);
//        for(int j = 1; j <= n; j++) {
//            fin >> a >> b;
//            treap.insert(b);
//        }
//        cout << "ROUND 1 :\theight : " << treap.height() << "          \tsize : " << n << '\n';
//    }
    std::cout << "Tests All Passed :)" << std::endl;
    return 0;
}
