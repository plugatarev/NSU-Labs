#include <iostream>
#include <time.h>
#include <cmath>
#include <x86intrin.h>
double pre_move(int* arr, const int size) {
    double minTime = UINT64_MAX;
    for (int i = 0; i < 100; i++) {
        double time = __rdtsc();
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
        time = __rdtsc() - time;
        if (arr[1] == 3) {
            std::cout << std::endl;
        }
        if (time < minTime) {
            minTime = time;
        }
    }
    return (double)minTime / size;
}

double post_move(int* arr, const int size) {
    double minTime = UINT64_MAX;
    for (int i = 0; i < 100; i++) {
        double time = __rdtsc();
        for (int i = size - 1; i > 0; i--) {
            arr[i] = i;
        }
        time = __rdtsc() - time;
        if (arr[size - 1] == 555) {
            std::cout << std::endl;
        }
        if (time < minTime) {
            minTime = time;
        }
    }
    return double(minTime) / size;
}

void random_pass_fill(int* arr, const int N) {
    for (int i = N - 1; i >= 1; --i) {
        int j = rand() % (i + 1);
        std::swap(arr[i], arr[j]);
    }
}

double rnd_move(int* arr, const int size) {
    srand(clock());
    double minTime = UINT64_MAX;
    double time = 0;
    random_pass_fill(arr, size);
    for (int i = 0; i < 7; i++){
        time = __rdtsc();
        for (volatile int k = 0, j = 0; j < size; ++j) {
            k = arr[k];
        }
        time = __rdtsc() - time;
        if (time < minTime){
            minTime = time;
        }
    } 
    return (minTime) / size;
}

void Count(const int size) {
    std::cout << size << std::endl;

    int* arr = new int[size];

    double preCountTime = pre_move(arr, size);

    double postCountTime = post_move(arr, size);

    double rndCountTime = rnd_move(arr, size);
    std::cout << preCountTime << std::endl;
    std::cout << postCountTime << std::endl;
    std::cout << rndCountTime << std::endl;

    delete[] arr;

    std::cout << "\n\n";
}

int main(){

    for (int i = 256; i < 8194304; i*=1.2) {
        Count(i);
    }
    return 0;
}
