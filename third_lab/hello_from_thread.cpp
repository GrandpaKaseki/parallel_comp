#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int n;
    cout << "Enter number of threads = ";
    cin >> n;
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        cout << "I am " << i << " thread from " << n << " threads!" << endl;
    }
  
    return 0;
}