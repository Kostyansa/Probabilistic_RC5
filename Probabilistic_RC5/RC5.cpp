//
//  RC5.cpp
//  Probabilistic_RC5
//
//  Created by Constantine on 02.11.2020.
//  Copyright © 2020 Kostyansa. All rights reserved.
//

#include "RC5.hpp"

using namespace RC5;

template<class T>
T cycleLeft(T x, unsigned int shift)
{
    shift %= sizeof(T)*CHAR_BIT;
    return (x << shift) | (x >> (sizeof(T)*CHAR_BIT - shift));
}

template<class T>
T cycleRight(T x, unsigned int shift)
{
    shift %= sizeof(T)*CHAR_BIT;
    return (x >> shift) | (x << (sizeof(T)*CHAR_BIT - shift));
}

std::pair<word, word> generateMagicNumbers(const unsigned int& width){
    word P;
    word Q;
    P = (word) ((RC5::e - 2) * ((ull) 1 << width));
    P += (1 - P % 2);
    Q = (word) ((RC5::phi - 2) * ((ull) 1 << width));
    Q -= (1 - Q % 2);
    return std::make_pair(P, Q);
}

std::vector<word> initializeSubkeys(
                                    const std::pair<ull, ull>& magic_numbers,
                                    const unsigned int & rounds
                                    ){
    std::vector<word> subkeys(2 * (rounds + 1));
    subkeys[0] = magic_numbers.first;
    for (long i = 1; i < subkeys.size(); i++) {
        subkeys[i] = subkeys[i - 1] + magic_numbers.second;
    }
    return subkeys;
}

std::vector<word> mixSubkeys(const std::vector<word>& subkeys, const std::vector<word>& key){
    word A = 0, B = 0;
    std::vector<word> s = subkeys;
    std::vector<word> k = key;
    for (long z = 0, i = 0, j = 0;
         z < 3*std::max(s.size(), k.size());
         z++,
         i = (i + 1) % s.size() ,
         j = (j + 1) % k.size()) {
        A = s[i] = cycleLeft((s[i] + A + B), 3);
        B = k[j] = cycleLeft((k[j] + A + B), A + B);
    }
    return s;
}

std::pair<word, word> encryptBlock(word left, word right, const std::vector<word>& subkeys){
    left += subkeys[0];
    right += subkeys[1];
    for (long i = 1; i < subkeys.size() / 2; i++) {
        left ^= right;
        left = cycleLeft(left, right);
        left += subkeys[2 * i];
        right ^= left;
        right = cycleLeft(right, left);
        right += subkeys[2 * i + 1];
        
        //Should work but doesn't:
//        left = cycleLeft(left ^ right, right) + subkeys[2 * i];
//        right = cycleLeft(right ^ left, left) + subkeys[2 * i + 1];
    }
    return std::make_pair(left, right);
}

std::pair<word, word> decryptBlock(word left, word right, const std::vector<word>& subkeys){
    for (long i = (subkeys.size() / 2) - 1; i >= 1; i--) {
        right -= subkeys[2 * i + 1];
        right = cycleRight(right, left);
        right ^= left;
        left -= subkeys[2 * i];
        left = cycleRight(left, right);
        left ^= right;
        
        //Should work but doesn't:
//        right = cycleRight(right - subkeys[2 * i + 1], left) ^ left;
//        left = cycleRight(left - subkeys[2 * i], right) ^ right;
    }
    right -= subkeys[1];
    left -= subkeys[0];
    return std::make_pair(left, right);
}

std::vector<word> RC5::encrypt(
                          const std::vector<word>& message,
                          const std::vector<word>& key,
                          const unsigned int& width,
                          const unsigned int& rounds
                          ){
    std::vector<word> encrypted(message.size());
    std::pair<word, word> magic_numbers = generateMagicNumbers(width);
    
    std::vector<word> subkeys = initializeSubkeys(magic_numbers, rounds);
    subkeys = mixSubkeys(subkeys, key);
    
    for (long i = 0; i < message.size()/2; i++) {
        std::pair<word, word> encrypted_block = encryptBlock(message[2 * i],
                                                              message[2 * i + 1],
                                                              subkeys);
        encrypted[2 * i] = encrypted_block.first;
        encrypted[2 * i + 1] = encrypted_block.second;
    }
    return encrypted;
}

std::vector<word> RC5::decrypt(
                          const std::vector<word>& message,
                          const std::vector<word>& key,
                          const unsigned int& width,
                          const unsigned int& rounds
                          ){
    std::vector<word> decrypted(message.size());
    std::pair<word, word> magic_numbers = generateMagicNumbers(width);
    
    std::vector<word> subkeys = initializeSubkeys(magic_numbers, rounds);
    subkeys = mixSubkeys(subkeys, key);
    
    for (long i = 0; i < message.size()/2; i++) {
        std::pair<word, word> decrypted_block = decryptBlock(message[2 * i],
                                                              message[2 * i + 1],
                                                              subkeys);
        decrypted[2 * i] = decrypted_block.first;
        decrypted[2 * i + 1] = decrypted_block.second;
    }
    return decrypted;
}
