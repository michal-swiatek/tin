//
// Created by micho6 on 09.01.2021.
//

#include <iostream>
#include <vector>

int main() {
    std::vector<char> v;

    char temp[] = {97, 98, 99, 100};

    v.insert(v.end(), temp, temp + 4);

    for(const auto& elem : v)
        std::cout << elem << ' ';

    return 0;
}
