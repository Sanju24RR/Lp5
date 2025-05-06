#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Generate random vector
vector<int> generate_vector(int size) {
    vector<int> vec(size);
    for (int &x : vec) x = rand() % 10000;
    return vec;
}

// Sequential Bubble Sort
void bubbleSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// Parallel Bubble Sort using OpenMP (Odd-Even Transposition Sort)
void parallelBubbleSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        int start = i % 2;
        #pragma omp parallel for
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// Sequential Merge Sort
void merge(vector<int> &arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;

    while (i < left.size() && j < right.size())
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

void mergeSort(vector<int> &arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Parallel Merge Sort using OpenMP tasks
void parallelMergeSort(vector<int> &arr, int l, int r, int depth = 0) {
    if (l >= r) return;
    int m = l + (r - l) / 2;

    if (depth <= 4) {
        #pragma omp task
        parallelMergeSort(arr, l, m, depth + 1);
        #pragma omp task
        parallelMergeSort(arr, m + 1, r, depth + 1);
        #pragma omp taskwait
    } else {
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
    }
    merge(arr, l, m, r);
}

int main() {
    const int SIZE = 35000;
    srand(time(0));

    vector<int> original = generate_vector(SIZE);

    // Sequential Bubble Sort
    vector<int> a1 = original;
    auto start = high_resolution_clock::now();
    bubbleSort(a1);
    auto end = high_resolution_clock::now();
    cout << "Sequential Bubble Sort: " << duration<double>(end - start).count() << "s\n";

    // Parallel Bubble Sort
    vector<int> a2 = original;
    start = high_resolution_clock::now();
    parallelBubbleSort(a2);
    end = high_resolution_clock::now();
    cout << "Parallel Bubble Sort:   " << duration<double>(end - start).count() << "s\n";

    // Sequential Merge Sort
    vector<int> a3 = original;
    start = high_resolution_clock::now();
    mergeSort(a3, 0, a3.size() - 1);
    end = high_resolution_clock::now();
    cout << "Sequential Merge Sort:  " << duration<double>(end - start).count() << "s\n";

    // Parallel Merge Sort
    vector<int> a4 = original;
    start = high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(a4, 0, a4.size() - 1);
    }
    end = high_resolution_clock::now();
    cout << "Parallel Merge Sort:    " << duration<double>(end - start).count() << "s\n";

    return 0;
}
