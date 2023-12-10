#pragma once
#ifndef _RANDOM_INTEGER_H
#define _RANDOM_INTEGER_H 1.0

#include "boost/multiprecision/cpp_int.hpp"
#include "boost/multiprecision/miller_rabin.hpp"
#include "boost/random.hpp"
#include <chrono>
#include <list>

typedef boost::random::mt19937_64 stdGenerator_t;
typedef boost::multiprecision::cpp_int Integer;

struct generateRange { const Integer _lowerBound; const Integer _upperBound;
generateRange(const Integer& left, const Integer& right) : _lowerBound(left), _upperBound(right) {
	if (_lowerBound >= _upperBound) throw std::invalid_argument("Invalid arguments for creating generateRange");
}
};

constexpr unsigned int millerRabinIterations = 20;

inline const bool isPrime(const Integer& number) {
	return boost::multiprecision::miller_rabin_test(number, millerRabinIterations);
}
template<typename Vc_t>
inline const Integer findPrime(const std::list<Vc_t>& list, bool& state) {
	for (const Vc_t& number : list) {
		if (isPrime(number)) {
			state = true;
			return number;
		}
	}
	return -1;
}

class _Random_integer_generator {
private:
	stdGenerator_t _generator;

	const Integer _getPrime(const generateRange& range);
public:
	template<size_t _BitSize>
	const Integer generateNumber();
	const Integer generateNumber(const generateRange& range);
	const Integer generateNumber(const Integer& _l, const Integer& _u);

	template<size_t _BitSize>
	const Integer generatePrime();
	const Integer generatePrime(const generateRange& range);
	const Integer generatePrime(const Integer& _l, const Integer& _u);

	_Random_integer_generator();
	_Random_integer_generator(const unsigned long long& seed);
	_Random_integer_generator(const stdGenerator_t _gen);
};
using Generator = _Random_integer_generator;

inline const Integer _Random_integer_generator::generatePrime(const Integer& _l, const Integer& _u) {
	return generatePrime(generateRange(_l, _u));
}
inline const Integer _Random_integer_generator::generateNumber(const Integer& _l, const Integer& _u) {
	return generateNumber(generateRange(_l, _u));
}

inline const Integer _Random_integer_generator::_getPrime(const generateRange& range) {
	boost::random::uniform_int_distribution<Integer> _dist(range._lowerBound, range._upperBound);
	bool state = false; 
	Integer candidate;
	std::list<Integer> _timedList(200);
	while (true) {
		for (Integer& number : _timedList) number = _dist(_generator) | 1;
		candidate = findPrime(_timedList, state);
		if (state) return candidate;
	}
}
inline const Integer _Random_integer_generator::generatePrime(const generateRange& range) {
	Integer candidate = generateNumber(range) | 1;
	if (isPrime(candidate)) return candidate;
	Integer _lowerBound = candidate / 6 - 2;
	Integer _upperBound = candidate * 6 + 2;
	if (_lowerBound < range._lowerBound) _lowerBound = range._lowerBound;
	if (_upperBound > range._upperBound) _upperBound = range._upperBound;
	bool _stat = (candidate - _lowerBound) < (_upperBound - candidate);
	return (_stat) ? _getPrime(generateRange(candidate, _upperBound)) : _getPrime(generateRange(_lowerBound, candidate));
}

template<size_t _BitSize>
inline const Integer _Random_integer_generator::generatePrime() {
	std::list<Integer> _timedList(200);
	boost::random::independent_bits_engine<boost::random::mt11213b, _BitSize, Integer> 
		_bitsEngine(_generator);
	bool state = false;
	Integer candidate;
	while (true) {
		for (Integer& number : _timedList) number = _bitsEngine() | 1;
		candidate = findPrime(_timedList, state);
		if (state) return candidate;
	}
}

inline const Integer
_Random_integer_generator::generateNumber(const generateRange& range){
	boost::random::uniform_int_distribution<Integer> _dist(range._lowerBound, range._upperBound);
	return _dist(_generator);
}

template<size_t _Bit_size>
inline const Integer
_Random_integer_generator::generateNumber() {
	boost::random::independent_bits_engine<stdGenerator_t, _Bit_size, Integer> _engine(_generator);
	return _engine();
}

inline _Random_integer_generator::_Random_integer_generator() {
	_generator = stdGenerator_t(std::chrono::steady_clock::now().time_since_epoch().count());
}
inline _Random_integer_generator::_Random_integer_generator(const unsigned long long& seed) : _generator(stdGenerator_t(seed)) {}
inline _Random_integer_generator::_Random_integer_generator(const stdGenerator_t _gen) : _generator(_gen) {}
#endif // !_RANDOM_INTEGER_H
