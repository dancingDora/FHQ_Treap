//
// Created by 陆逸凡 on 2023/3/30.
//
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    string filename = "./analyse/test";
    for (int j = 1; j <= 20; j++) {
        filename =  "./analyse/test";
        filename += to_string(j);
        ofstream fout(filename);
        int n = 500 * j;
        for (int i = 1; i <= n; i++) {
            fout << 1 << " " << rand() << endl;
        }
    }
    return 0;
}