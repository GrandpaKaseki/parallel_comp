#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>
#include <chrono>

using namespace std;

mutex myMutex;


void write_to_file(int id) {
    unique_lock<mutex> lock(myMutex);
    fstream ofs( "result.txt", ios::app);
    cout << "Hello it`s thread "<<id<<"\n";
    ofs << "Hello it`s thread "<<id<<"\n";
    std::chrono::milliseconds timespan(1605);
    std::this_thread::sleep_for(timespan);
    cout<<"Bye from thread "<<id<<"\n";
    ofs<<"Bye from thread "<<id<<"\n";
    ofs.close();
    lock.unlock(); 
}

 
int main() {
    const int processor_count = std::thread::hardware_concurrency();

    fstream ofs;
    ofs.open("result.txt", fstream::out | fstream::trunc);
    ofs.close();
    
    thread t[processor_count];
     for(int i = 0; i < processor_count; i++){
         t[i]= thread(write_to_file,i);
     }

    for(int i = 0; i < processor_count; i++){
            t[i].join();
        }

    return 0;
}
