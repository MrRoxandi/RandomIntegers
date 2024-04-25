#pragma once
#ifndef _RANDOM_INTEGER_H
#define _RANDOM_INTEGER_H 1.0

#include "boost/multiprecision/miller_rabin.hpp"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/random.hpp"
#include <type_traits>
#include <chrono>
#include <string>
#include <bitset>
#include <cstdint>

/* This struct will help us to check valid input type */
template<typename inType>
struct isInteger {
	constexpr static bool value = std::is_integral<inType>::value || std::is_same<inType, boost::multiprecision::cpp_int>::value;
};

/* Some usefull macros */

#define validate(x) static_assert(isInteger<x>::value, "Invalid input type. " #x " - isn't integral")

#define int_dist(inT, lb, ub) boost::random::uniform_int_distribution<inT> dist(lb, ub)

#define testIterations 20

#define rabinTest(a) boost::multiprecision::miller_rabin_test(a, testIterations)

//Some usefull functions:
template<class Integer>
Integer mulmod(Integer a, Integer b, Integer m);

template<class Integer>
Integer powmod(Integer a, Integer b, Integer m);

template<class Integer>
bool isPrime(Integer number);

template<class Integer>
Integer abs(Integer number);

template<class Integer>
Integer binToDec(const std::string_view view);

template<class Integer>
std::string decToBin(Integer number);

template<class Integer>
size_t bitLen(Integer number);

static boost::random::mt19937_64 genEngine(
	std::random_device{}()
	//std::chrono::steady_clock::now().time_since_epoch().count()
);

//Some first primes lower then 400
std::vector<uint16_t> first_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397 };

//Some functions to get Random Integers

template<class Integer, size_t BitLen>
static const Integer getRandomBits();

template<class Integer>
static const Integer getRandomBits(const size_t len);

template<class Integer, size_t BitLen>
const Integer getRandomInteger();

template<class Integer, size_t BitLen>
const Integer getRandomPrime();

template<class Integer>
const Integer getRandomInteger(const Integer lower_bound, const Integer upper_bound);

template<class Integer>
const Integer getRandomPrime(const Integer lower_bound, const Integer upper_bound);

//Realisations for RandomGenerator methods

template<class Integer>
inline const Integer getRandomPrime(const Integer lower_bound, const Integer upper_bound)
{
	validate(Integer); //Validating types;
	//boost::random::uniform_int_distribution<Integer> dist(lower_bound, upper_bound);
	int_dist(Integer, lower_bound, upper_bound);
	Integer candidate;
	for(;;)
	{ 
		candidate = dist(genEngine) | 1;
		if (isPrime(candidate))
			return candidate;
	}
}

template<class Integer>
inline const Integer getRandomInteger(const Integer lower_bound, const Integer upper_bound)
{
	//boost::random::uniform_int_distribution<Integer> dist(lower_bound, upper_bound);
	validate(Integer);
	int_dist(Integer, lower_bound, upper_bound);
	return dist(genEngine);
}

template<class Integer, size_t BitSize>
inline const Integer getRandomPrime()
{
	validate(Integer);
	Integer candidate;
	while (true)
	{
		candidate = getLoweLevelPrime<Integer, BitSize>();
		if (isPrime(candidate))
			return candidate;
	}
}

template<class Integer, size_t BitSize>
inline const Integer getLoweLevelPrime()
{
	validate(Integer);
	while (true)
	{
		Integer candidate = getRandomBits<Integer, BitSize>();
		bool is_prime = true;
		for (int i = 0; i < first_primes.size(); ++i)
		{
			if (candidate == first_primes[i])
				return candidate;
			if (candidate % first_primes[i] == 0)
			{
				is_prime = false;
				break;
			}
		}
		if (is_prime) return candidate;
	}
}

template<class Integer>
inline const Integer getLoweLevelPrime(const size_t len)
{
	validate(Integer);
	while (true)
	{
		Integer candidate = getRandomBits<Integer>(len);
		bool is_prime = true;
		for (int i = 0; i < first_primes.size(); ++i)
		{
			if (candidate == first_primes[i])
				return candidate;
			if (candidate % first_primes[i] == 0)
			{
				is_prime = false;
				break;
			}
		}
		if (is_prime) return candidate;
	}
}

template<class Integer, size_t BitSize>
inline const Integer getRandomInteger()
{
	return getRandomBits<Integer, BitSize>();
}

template<class Integer, size_t BitSize>
inline const Integer getRandomBits()
{
	validate(Integer);
	std::bitset<BitSize - 1> BitSet;
	int_dist(uint16_t, 0, 1);
	for (size_t i = 0; i < BitSize; ++i)
		BitSet[i] = dist(genEngine);
	BitSet[0] = 1;
	BitSet[BitSize - 2] = 1;
	return binToDec<Integer>(BitSet.to_string());
}

template<class Integer>
inline const Integer getRandomBits(const size_t len)
{
	validate(Integer);
	std::string bits(len, 0);
	int_dist(uint16_t, 0, 1);
	for (size_t i = 0; i < len - 1; ++i)
		bits[i] = static_cast<char>('0' + dist(genEngine));
	bits[len - 1] = '1';
	return binToDec<Integer>(bits);
}

//Realisations for functions

template<class Integer>
inline Integer mulmod(Integer a, Integer b, Integer m)
{
	//Type check;
	validate(Integer);
	Integer result = 0;
	while (a != 0)
	{
		if (a & 1)
			result = (result + b) % m;
		a >>= 1;
		b = (b << 1) % m;
	}
	return result;
}

template<class Integer>
inline Integer powmod(Integer a, Integer b, Integer m)
{
	//Type check;
	validate(Integer);
	Integer result = 1;
	a %= m;
	while (b > 0)
	{
		if (!(b & 1))
		{
			result = mulmod(result, a, m);
		}
		a = mulmod(a, a, m);
		b >>= 1;
	}
	return result % m;
}

template<class Integer>
inline bool isPrime(Integer a)
{
	//Type check;
	validate(Integer);
	return rabinTest(a);
}

template<class Integer>
Integer binToDec(const std::string_view view)
{
	//Type check;
	validate(Integer);
	Integer result = 0;
	for (size_t i = 0; i < view.length(); ++i)
	{
		result <<= 1;
		result += static_cast<int>(view[i] - '0');
	}
	return result;
}

template<class Integer>
std::string decToBin(Integer number)
{
	//Type check;
	validate(Integer);
	std::string binary = "";
	for (; number > 0; number >>= 1)
		binary = (number & 1 ? "1" : "0") + binary;
	return binary;
}


template<class Integer>
size_t bitLen(Integer number)
{
	//Type check;
	validate(Integer);
	size_t len = 0;
	for (; number > 0; number >>= 1)
		len += 1;
	return len;
}

template<class Integer>
Integer abs(Integer number)
{
	//Type check;
	validate(Integer);
	return (number < 0) ? -number : number;
}
#endif // !_RANDOM_INTEGER_H