#pragma once
#ifndef _RANDOM_INTEGER_H
#define _RANDOM_INTEGER_H 1.0

#include "boost/multiprecision/miller_rabin.hpp"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/random.hpp"
#include <type_traits>
#include <execution>
#include <iterator>
#include <optional>
#include <chrono>
#include <string>
#include <deque>
#include <bitset>
#include <cstdint>


//Type check struct
template<typename inType>
struct isInteger {
	constexpr static bool value = std::is_integral<inType>::value || std::is_same<inType, boost::multiprecision::cpp_int>::value;
};

//Some usefull functions:
template<class Integer>
Integer mulmod(Integer a, Integer b, Integer m);

template<class Integer>
Integer powmod(Integer a, Integer b, Integer m);

template<class Integer>
bool isPrime(Integer a);

template<class Integer>
Integer binToDec(const std::string_view view);

template<class Integer>
std::string decToBin(Integer number);

template<class Integer>
size_t bitLen(Integer number);

constexpr short testIterations = 20;

//Some first primes lower then 1'000
std::vector<uint16_t> first_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397 };

template<class Integer>
class RandomGenerator
{
	static_assert(isInteger<Integer>::value, "Generator type must be an integer");
private:
	boost::random::mt19937_64 genEngine;

	template<size_t BitSize>
	const Integer getLoweLevelPrime();

	const Integer getLoweLevelPrime(const size_t len);

	template<size_t BitSize>
	const Integer getRandomBits();

	const Integer getRandomBits(const size_t len);

public:
	RandomGenerator();
	RandomGenerator(const size_t seed);
	RandomGenerator(const boost::random::seed_seq& seq);


	template<size_t BitSize>
	const Integer getRandomInteger();

	template<size_t BitSize>
	const Integer getRandomPrime();

	const Integer getRandomInteger(const Integer lower_bound, const Integer upper_bound);

	const Integer getRandomPrime(const Integer lower_bound, const Integer upper_bound);

	~RandomGenerator() noexcept = default;
};


//Realisations for RandomGenerator methods

template<class Integer>
inline const Integer RandomGenerator<Integer>::getRandomPrime(const Integer lower_bound, const Integer upper_bound)
{
	size_t lower_Size = bitLen(lower_bound);
	size_t upper_Size = bitLen(upper_bound);
	boost::random::uniform_int_distribution<size_t> dist(lower_Size, upper_Size);
	size_t bit_len = dist(genEngine);
	Integer candidate;
	while (true)
	{
		candidate = getLoweLevelPrime(bit_len);
		if (isPrime(candidate))
			return candidate;
	}
}

template<class Integer>
inline const Integer RandomGenerator<Integer>::getRandomInteger(const Integer lower_bound, const Integer upper_bound)
{
	size_t lower_Size = bitLen(lower_bound);
	size_t upper_Size = bitLen(upper_bound);
	boost::random::uniform_int_distribution<size_t> dist(lower_Size, upper_Size);
	size_t bit_len = dist(genEngine);
	return getRandomBits(bit_len);
}

template<class Integer> template<size_t BitSize>
inline const Integer RandomGenerator<Integer>::getRandomPrime()
{
	Integer candidate;
	while (true)
	{
		candidate = getLoweLevelPrime<BitSize>();
		if (isPrime(candidate))
			return candidate;
	}
}

template<class Integer> template<size_t BitSize>
inline const Integer RandomGenerator<Integer>::getLoweLevelPrime()
{
	while (true)
	{
		Integer candidate = getRandomBits<BitSize>();
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
inline const Integer RandomGenerator<Integer>::getLoweLevelPrime(const size_t len)
{
	while (true)
	{
		Integer candidate = getRandomBits(len);
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

template<class Integer> template<size_t BitSize>
inline const Integer RandomGenerator<Integer>::getRandomInteger()
{
	return getRandomBits<BitSize>();
}

template<class Integer> template<size_t BitSize>
inline const Integer RandomGenerator<Integer>::getRandomBits()
{
	std::bitset<BitSize - 1> BitSet;
	boost::random::uniform_int_distribution<unsigned short> dist(0, 1);
	for (size_t i = 0; i < BitSize; ++i)
		BitSet[i] = dist(genEngine);
	BitSet[0] = 1;
	BitSet[BitSize - 2] = 1;
	return binToDec<Integer>(BitSet.to_string());
}

template<class Integer>
inline const Integer RandomGenerator<Integer>::getRandomBits(const size_t len)
{
	std::string bits = {};
	bits.resize(len - 1, '0');
	boost::random::uniform_int_distribution<unsigned short> dist(0, 1);
	for (size_t i = 0; i < len - 1; ++i)
		bits[i] = static_cast<char>('0' + dist(genEngine));
	bits[len - 1] = '1';
	return binToDec<Integer>(bits);
}

template<class Integer>
inline RandomGenerator<Integer>::RandomGenerator()
{
	genEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
}

template<class Integer>
inline RandomGenerator<Integer>::RandomGenerator(const size_t seed)
{
	genEngine.seed(seed);
}

template<class Integer>
inline RandomGenerator<Integer>::RandomGenerator(const boost::random::seed_seq& seq)
{
	genEngine.seed(seq);
}

//Realisations for functions

template<class Integer>
inline Integer mulmod(Integer a, Integer b, Integer m)
{
	//Type check;
	static_assert(isInteger<Integer>::value, "Input type must be an integral");
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
	static_assert(isInteger<Integer>::value, "Input type must be an integral");
	Integer result = 1;
	a %= m;
	while (b > 0)
	{
		if (b % 2 == 1)
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
	static_assert(isInteger<Integer>::value, "Input type must be an integral");
	return boost::multiprecision::miller_rabin_test(a, testIterations);
}

template<class Integer>
Integer binToDec(const std::string_view view)
{
	//Type check;
	static_assert(isInteger<Integer>::value, "Input type must be an integral");
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
	static_assert(isInteger<Integer>::value, "Input type must be an integral");
	std::string binary = "";
	for (; number > 0; number >>= 1)
		binary = (number & 1 ? "1" : "0") + binary;
	return binary;
}


template<class Integer>
size_t bitLen(Integer number)
{
	size_t len = 0;
	for (; number > 0; number >>= 1)
		len += 1;
	return len;
}
#endif // !_RANDOM_INTEGER_H