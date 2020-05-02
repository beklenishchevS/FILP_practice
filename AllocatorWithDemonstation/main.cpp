#include <cstddef>
#include <cstdlib>
#include <map>
#include <list>
#include <cmath>
#include <iostream>
#include "Allocator.cpp"
using namespace std;


int main() {
    auto* a = new Allocator(1024);
    auto idx = (size_t)a->alloc(128);
    cout << idx - a->startOfMem << endl;
    auto idx2 = (size_t)a->alloc(128);
    cout << idx2 - a->startOfMem << endl;
    a->dealloc((void*) idx);
    auto idx3 = (size_t)a->alloc(128);
    cout << idx3 - a->startOfMem << endl;
    return 0;
}
