/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: dima
 *
 * Created on 6 октября 2017 г., 17:00
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <chrono>
#include <vector>
#include <unordered_map>

int k = 10, THRESH = 4;


struct entry {
    std::string word;
    int count;
    bool operator<(const entry& rhs ) {
        return this->count > rhs.count;
    }
};

int main(int argc, char** argv) {
    
    int k = atoi(argv[1]);
    int THRESH = atoi(argv[2]);
    
    std::vector<std::string> words;
    std::vector<std::string> origin;
    
    std::ifstream file;
    //file.open ("/home/dima/PycharmProjects/untitled/res.txt");
    file.open ("/home/dima/Загрузки/bible.txt");
    std::string word;
    while (file >> word)
    {
        origin.push_back(word);
    }
    file.close();

    words = origin;
    
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();    
    
    std::sort(words.begin(), words.end());
    int count = 1;
    
    auto w_it = words.begin();
    word = *w_it;
    ++w_it;
    std::vector<entry> entries;
    
    while (w_it != words.end())
    {
        if (*w_it != word) {
            entry e = {word, count};
            entries.push_back(e);
            count = 1;
            word = *w_it;
        } else {
            ++count;
        }
        ++w_it;
    }
    
    std::sort(entries.begin(), entries.end());
    
    std::vector<entry> naive_result;
    
    for (auto it = entries.begin(); (it != entries.end()) && (naive_result.size() < k); ++it)
    {
        if ((*it).count >= THRESH )
            naive_result.push_back(*it);
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;
    
    std::cout << "Naive method. Calculated in " << time_span.count() << "ms" << std::endl;
    
    for (auto it = naive_result.begin(); it != naive_result.end(); ++it)
    {
        std::cout << (*it).word << " : " << (*it).count << std::endl;
    }
    
    //-------------------------------------------------------
    
    std::unordered_map<std::string, int> map;
    
    t1 = high_resolution_clock::now();
    
    for (auto it = origin.begin(); it != origin.end(); ++it)
    {
        auto map_it = map.find(*it);
        if (map_it == map.end())
        {
            map.insert(std::pair<std::string, int> (*it, 1) );
        } else {
            (*map_it).second++;
        }
    }
    
    std::vector<entry> res;
    res.reserve(k);
    
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        entry e = {(*it).first, (*it).second};
        res.push_back(e);
    }
    
    auto cmp = [](entry const & a, entry const & b) 
    { 
         return a.count > b.count;
    };
    std::sort(res.begin(), res.end(), cmp);

    t2 = high_resolution_clock::now();
    time_span = t2 - t1;
    std::cout << "Map method. Calculated in " << time_span.count() << "ms" << std::endl;
    
    int c = 0;
    for (auto it = res.begin(); it != res.end(); ++it)
    {
        if (c < k && (*it).count >= THRESH)
        {
            std::cout << (*it).word << " : " << (*it).count << std::endl;
            c++;
        }
    }
    
    return 0;
}

