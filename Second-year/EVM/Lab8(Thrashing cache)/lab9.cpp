#include <cstdlib>
#include <iostream>
#include <x86intrin.h>

using namespace std;

//TODO: Changed Cache
#define L1_cache 131072 // 128 KB
#define L2_cache 2097152 // 2 MB
#define L3_cache 4194304 // 4 MB

double count(int *arr, size_t size) {    
    double time = __rdtsc();
    for (volatile size_t k = 0, i = 0; i < 10 * size; ++i) {
        k = arr[k];
    }
    time = __rdtsc() - time;
    return time / (10 * size);
}

void fill_array(int* K, int const size, int n){
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < n - 1; ++j) {
            K[j * size + i] = (j + 1) * size + i;
        }
        K[(n - 1) * size + i] = (i + 1) % size;
    }
}

int main() {
    int const size = (L2_cache) / 4;
    int* K = new int[70 * size];
    for (int i = 1; i < 70; ++i){
        fill_array(K, size, i);
        cout << i << "   " << count(K, size * i) << endl;
    }
    delete K;
    return 0;
}