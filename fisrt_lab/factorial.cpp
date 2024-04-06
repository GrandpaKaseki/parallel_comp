#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex myMutex;

void part_of_factorial(int start,int stop,  unsigned long long& result){ 
    // calculates part of factorial
    int local_result = 1;

    if (start == 0){
        start = 1;
    }

    for (int i = start; i < stop; i++){
        local_result = local_result * i;
    }

    lock_guard<mutex> lock(myMutex); 
    result = result * local_result; // it may be a race condition for wrting into this variable

}

int main(){
    const int processor_count = std::thread::hardware_concurrency(); // check how many threads can support our CPU
    
    int factorial_arg; // target number which used to calculate factorial value
    cout << "\nEnter number\n";
    cin >> factorial_arg;
    auto begin_time = chrono::steady_clock::now();
    int step = factorial_arg / processor_count; // how many numbers will multiply every thread
    unsigned long long result = 1;

    if (step < 1){ // if argument is less then number of cores, it`s more efficient to create only one thread then create a race condition for multiplying
        thread AloneWolf(part_of_factorial,1,factorial_arg+1,ref(result));
        AloneWolf.join();
    }

    else{
    
        thread t[processor_count]; // array for threads
        for(int i = 0; i < processor_count; i++){
            int start = step*i;
            int stop = step*(i+1);
            if ((i+1)==processor_count){ // we need to be careful to not miss last number of factorial
                stop = factorial_arg + 1;
            }
            t[i]= thread(part_of_factorial,start,stop,ref(result));
        }

        // Wait for threads, which are running concurrently, to finish
        for(int i = 0; i < processor_count; i++){
            t[i].join();
        }
    }
    auto end_time = chrono::steady_clock::now();
    auto elapsed_ms = chrono::duration_cast<chrono::nanoseconds>(end_time - begin_time);
    cout << "Result:" << result << endl;
    cout << "Work time:" << elapsed_ms.count()/1000000 << "ms" << endl;
    return 0;
}