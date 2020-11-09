//
//  RC5.hpp
//  Probabilistic_RC5
//
//  Created by Constantine on 02.11.2020.
//  Copyright © 2020 Kostyansa. All rights reserved.
//

#ifndef RC5_hpp
#define RC5_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


namespace RC5 {

typedef unsigned long long ull;

typedef unsigned short int word;

const double e = exp(1);
const double phi = (1 + sqrt(5)) / 2;

std::vector<word> encrypt(const std::vector<word>& message, const std::vector<word>& key, const unsigned int& width, const unsigned int& rounds);

std::vector<word> decrypt(const std::vector<word>& message, const std::vector<word>& key, const unsigned int& width, const unsigned int& rounds);
}

#endif /* RC5_hpp */
