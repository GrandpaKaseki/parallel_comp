#include <iostream>
#include <omp.h>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

int main()
{
    setlocale(0, "");
    string line;
    vector<string> pups;
    ifstream in("hobbitses.txt"); // окрываем файл для чтения
    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            if (line == "") {
                continue;
            }
            else {
                pups.push_back(line);
            }

        }
    }
    else {
        cout << "Huynya";
    }
    in.close();     // закрываем файл

    map<string, int> word_counter;
    map<string, int>::iterator it;
    vector<string> keys;

    #pragma omp parallel for 
    for (int i = 0; i < pups.size(); i++) {
        stringstream ss{ pups[i] };
        string word;
        while (ss >> word) {
            transform(word.begin(), word.end(), word.begin(),
                [](unsigned char c) { return std::tolower(c); });

            it = word_counter.find(word);
            if (it != word_counter.end()) {
                #pragma omp atomic
                word_counter[word] ++;
            }
            else {
                #pragma omp critical
                word_counter[word] = 1;
                keys.push_back(word);
            }
        }
    }

    int max_counter = 1;
    string freq_word;
    for (int i = 0; i < keys.size(); i++) {
        if (word_counter[keys[i]] > max_counter) {
            max_counter = word_counter[keys[i]];
            freq_word = keys[i];
        }
    }
        
    cout << "Word \""<<freq_word<<"\" has apeared "<<max_counter<<" times in text!"<<endl;
    

}