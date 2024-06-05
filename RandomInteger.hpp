#pragma once
#ifndef RANDOM_INTEGER_H
#define RANDOM_INTEGER_H 1.1

#include "boost/multiprecision/miller_rabin.hpp"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/random.hpp"
#include <concepts>
#include <bitset>
#include <string>
#include <array>

static boost::random::mt19937_64 genEngine{std::random_device{}()};

//Some first primes lower then 400
constexpr std::array first_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
                                      101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
                                      199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
                                      317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397 };

template<typename T>
concept Integral = std::is_integral_v<T> || std::is_same_v<T, boost::multiprecision::cpp_int>;

constexpr auto testIterations = 20;

template<Integral Integer>
Integer mulMod(Integer a, Integer b, Integer m){
    Integer result = 0;
    for(; a != 0; a >>= 1){
        if(a & 1) result = (result + b) % m;
        b = (b << 1) % m;
    }
    return result;
}

template<Integral Integer>
Integer powMod(Integer a, Integer b, Integer m){
    Integer result = 1; a %= m;
    for(; b > 0; b >>= 1){
        if(b & 1) result = mulMod(result, a, m);
        a = mulMod(a, a, m);
    }
    return result % m;
}

template<Integral Integer> bool isPrime(Integer number){
    return boost::multiprecision::miller_rabin_test(number, testIterations);
}

template<Integral Integer>
Integer binToDec(const std::string_view view){
    Integer result = 0;
    for (const auto &item: view){
        result <<= 1;
        result += (int)(item - '0');
    }
    return result;
}

template<Integral Integer>
std::string decToBin(Integer number){
    std::stringstream binaryStream;
    for (; number > 0; number >>= 1)
        binaryStream.putback((number & 1) ? '1' : '0');
    return binaryStream.str();
}

template<Integral Integer>
size_t bitCount(Integer number){
    size_t len = 0;
    for (; number > 0; number >>= 1)
        len += 1;
    return len;
}

//Some functions to get Random Integers

template<Integral Integer, size_t BitSize>
Integer getRandomBits(){
    std::bitset<BitSize - 1> BitSet;
    boost::random::uniform_int_distribution<int> intDistribution(0, 1);
    for (size_t i = 0; i < BitSize; ++i)
        BitSet[i] = intDistribution(genEngine);
    BitSet[0] = 1;
    BitSet[BitSize - 2] = 1;
    return binToDec<Integer>(BitSet.to_string());
}


template<Integral Integer, size_t bitCount>
Integer getRandomInteger(){
    return getRandomBits<Integer, bitCount>();
}

template<Integral Integer>
Integer getRandomInteger(Integer lower_bound, Integer upper_bound){
    boost::random::uniform_int_distribution<Integer> intDistribution(lower_bound, upper_bound);
    return intDistribution(genEngine);
}

template<Integral Integer, size_t bitCount>
Integer getLowLevelPrime(){
    Integer candidate;
    while (true){
        candidate = getRandomBits<Integer, bitCount>();
        bool is_prime = true;
        for (const auto &item: first_primes){
            if(candidate == item) return candidate;
            else if(candidate % item == 0){
                is_prime = false;
                break;
            }
        }
        if(is_prime) return candidate;
    }
}

template<Integral Integer>
Integer getLowLevelPrime(Integer lower_bound, Integer upper_bound){
    Integer candidate;
    while(true){
        candidate = getRandomInteger(lower_bound, upper_bound) | 1;
        bool is_prime = true;
        for (const auto &item: first_primes){
            if(candidate == item) return candidate;
            else if(candidate % item == 0){
                is_prime = false;
                break;
            }
        }
        if(is_prime) return candidate;
    }
}

template<Integral Integer, size_t BitSize>
Integer getRandomPrime() {
    Integer candidate;
    while (true){
        candidate = getLowLevelPrime<Integer, BitSize>();
        if (isPrime(candidate))
            return candidate;
    }
}

template<Integral Integer>
Integer getRandomPrime(Integer lower_bound, Integer upper_bound){
    Integer candidate;
    while(true){
        candidate = getLowLevelPrime(lower_bound, upper_bound);
        if (isPrime(candidate))
            return candidate;
    }
}

#endif // !RANDOM_INTEGER_H