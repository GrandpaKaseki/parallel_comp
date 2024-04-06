#include <iostream>
#include <omp.h>
#include <thread>
using namespace std;

int main() {
    int k = 100;
    int* arr = new int[k];

    for (int i; i< k; i++)
    {
        arr[i] = 2;
    }

    int sum = 0;
#pragma omp parallel reduction (+:sum)
    {
    #pragma omp for
    for (int i = 0; i<k;i++)
    {
        sum +=arr[i];
    }
    }
    cout << sum <<endl;

    return 0;
}