//
//  main.cpp
//  Probabilistic_RC5
//
//  Created by Constantine on 02.11.2020.
//  Copyright © 2020 Kostyansa. All rights reserved.
//

#include <iostream>
#include "RC5.hpp"

std::vector<RC5::word> probabilistic_encrypt(const std::vector<RC5::word>& message, const std::vector<RC5::word>& key, const unsigned int& width, const unsigned int& rounds){
    
    std::vector<RC5::word> probabilistic_message(message.size() * 2, 0);
    
    for (int i = 0; i < message.size(); i++) {
        probabilistic_message[2*i] = message[i];
        probabilistic_message[2*i + 1] = (RC5::word) rand();
    }
    
    return RC5::encrypt(probabilistic_message, key, width, rounds);
}

std::vector<RC5::word> probabilistic_decrypt(const std::vector<RC5::word>& message, const std::vector<RC5::word>& key, const unsigned int& width, const unsigned int& rounds){
    
    std::vector<RC5::word> probabilistic_message = RC5::decrypt(message, key, width, rounds);
    std::vector<RC5::word> decrypted(message.size() / 2);
    
    for (int i = 0; i < message.size()/2; i++) {
        decrypted[i] = probabilistic_message[2*i];
    }
    
    return decrypted;
}

std::vector<RC5::word> inputWords(const unsigned int& size){
    std::vector<RC5::word> words;
    for (long i = 0; i < size; i++) {
        RC5::word input;
        std::cin >> input;
        words.push_back(input);
    }
    return words;
}

int main(int argc, const char * argv[]) {
    const unsigned int key_length = 8;
    //If you want to change the width parameter you should change typedef word to the needed size in RC5.hpp
    const unsigned int width = 16;
    const unsigned int rounds = 16;
    
    std::cout << "Probabilistic cipher algorithm based on RC5-" << width/CHAR_BIT << "/" << rounds << "/" << key_length << std::endl;
    
    int n;
    bool finished = false;
    do {
        std::cout << "Input 1 to Encrypt message" << std::endl;
        std::cout << "Input 2 to Decrypt message" << std::endl;
        std::cout << "Input 3 to exit" << std::endl;
        std::cin.clear();
        std::cin >> n;
        switch (n) {
            case 1:{
                unsigned int size;
                std::cout << "Input size of your message in blocks of " << width/CHAR_BIT << " bytes" << std::endl;
                std::cin.clear();
                std::cin >> size;
                std::cout << "Input your message in blocks of " << width/CHAR_BIT << " bytes divided by spaces" << std::endl;
                std::cin.clear();
                std::vector<RC5::word> message = inputWords(size);
                std::cout << "Input your key size " << key_length <<" bytes in blocks of " << width/CHAR_BIT << " bytes divided by spaces" << std::endl;
                std::cin.clear();
                std::vector<RC5::word> key = inputWords(key_length/(width/CHAR_BIT));
                std::vector<RC5::word> encrypted = probabilistic_encrypt(message, key, width, rounds);
                std::cout << "Your encrypted message: " << std::endl;
                for (int i = 0; i < encrypted.size(); i++) {
                    std::cout << encrypted[i] << " ";
                }
                std::cout << std::endl;
                break;
            }
            case 2:{
                unsigned int size;
                std::cout << "Input size of your message in blocks of " << width/CHAR_BIT << " bytes" << std::endl;
                std::cin.clear();
                std::cin >> size;
                std::cout << "Input your message in blocks of " << width/CHAR_BIT << " bytes divided by spaces" << std::endl;
                std::cin.clear();
                std::vector<RC5::word> message = inputWords(size);
                std::cout << "Input your key size " << key_length <<" in blocks of " << width/CHAR_BIT << " bytes divided by spaces" << std::endl;
                std::cin.clear();
                std::vector<RC5::word> key = inputWords(key_length/(width/CHAR_BIT));
                std::vector<RC5::word> decrypted = probabilistic_decrypt(message, key, width, rounds);
                std::cout << "Your decrypted message: " << std::endl;
                for (int i = 0; i < decrypted.size(); i++) {
                    std::cout << decrypted[i] << " ";
                }
                std::cout << std::endl;
                break;
            }
            case 3:{
                finished = true;
                break;
            }
            default:{
                std::cout << "Invalid command" << std::endl;
            }
        }
    } while (!finished);
    return 0;
}
