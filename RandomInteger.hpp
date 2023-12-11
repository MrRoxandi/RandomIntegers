#pragma once
#ifndef _RANDOM_INTEGER_H
#define _RANDOM_INTEGER_H 1.0

#include "boost/multiprecision/miller_rabin.hpp"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/random.hpp"
#include <optional>
#include <chrono>
#include <list>

typedef boost::random::mt19937_64 stdGenerator_t;
typedef boost::multiprecision::cpp_int Integer;

template<typename R_t = Integer>
struct generateRange { const R_t _lowerBound; const R_t _upperBound;
generateRange(const R_t& left, const R_t& right) : _lowerBound(left), _upperBound(right) {
	if (_lowerBound >= _upperBound) throw std::invalid_argument("Invalid arguments for creating generateRange");
}
};

constexpr unsigned int millerRabinIterations = 20;

template<typename _Ch_t>
inline const bool isPrime(const _Ch_t& number) {
	return boost::multiprecision::miller_rabin_test(number, millerRabinIterations);
}

template<typename Vc_t>
inline const std::optional<Vc_t> findPrime(std::list<Vc_t>& list) {
	for (
		auto ptr = list.begin();
		ptr != list.end();
		++ptr
		)
		if (isPrime<Vc_t>(*ptr))
			return std::optional<Vc_t>(*ptr);
	return std::optional<Vc_t>();
}

template<typename R_t = Integer>
class _Random_integer_generator {
private:
	stdGenerator_t _generator;

	const R_t _getPrime(const generateRange<R_t>& range);
public:
	template<size_t _BitSize>
	const R_t generateNumber();
	const R_t generateNumber(const generateRange<R_t>& range);
	const R_t generateNumber(const R_t& _l, const R_t& _u);

	template<size_t _BitSize>
	const R_t generatePrime();
	const R_t generatePrime(const generateRange<R_t>& range);
	const R_t generatePrime(const R_t& _l, const R_t& _u);

	_Random_integer_generator();
	_Random_integer_generator(const unsigned long long& seed);
	_Random_integer_generator(const stdGenerator_t _gen);
};

template<typename R_t>
using Generator = _Random_integer_generator<R_t>;

template<typename R_t>
inline const R_t _Random_integer_generator<R_t>::generatePrime(const R_t& _l, const R_t& _u) {
	return generatePrime(generateRange<R_t>(_l, _u));
}

template<typename R_t>
inline const R_t _Random_integer_generator<R_t>::generateNumber(const R_t& _l, const R_t& _u) {
	return generateNumber(generateRange<R_t>(_l, _u));
}

template<typename R_t>
inline const R_t _Random_integer_generator<R_t>::_getPrime(const generateRange<R_t>& range) {
	boost::random::uniform_int_distribution<R_t> _dist(range._lowerBound, range._upperBound);
	std::optional<R_t> candidate;
	std::list<R_t> _timedList(200);
	while (true) {
		for (R_t& number : _timedList) number = _dist(_generator) | 1;
		candidate = findPrime<R_t>(_timedList);
		if (candidate.has_value())
			return candidate.value();
	}
}
template<typename R_t>
inline const R_t _Random_integer_generator<R_t>::generatePrime(const generateRange<R_t>& range) {
	R_t candidate = generateNumber(range) | 1;
	if (isPrime(candidate)) return candidate;
	R_t _lowerBound = candidate / 6 - 2;
	R_t _upperBound = candidate * 6 + 2;
	if (_lowerBound < range._lowerBound) _lowerBound = range._lowerBound;
	if (_upperBound > range._upperBound) _upperBound = range._upperBound;
	bool _stat = (candidate - _lowerBound) < (_upperBound - candidate);
	return (_stat) ? _getPrime(generateRange(candidate, _upperBound)) : _getPrime(generateRange(_lowerBound, candidate));
}

template<typename R_t> template<size_t _BitSize>
inline const R_t _Random_integer_generator<R_t>::generatePrime() {
	std::list<R_t> _timedList(200);
	boost::random::independent_bits_engine<boost::random::mt11213b, _BitSize, R_t> 
		_bitsEngine(_generator);
	std::optional<R_t> candidate;
	while (true) {
		for (R_t& number : _timedList) number = _bitsEngine() | 1;
		candidate = findPrime<R_t>(_timedList);
		if (candidate.has_value()) 
			return candidate.value();
	}
}

template<typename R_t> inline const R_t
_Random_integer_generator<R_t>::generateNumber(const generateRange<R_t>& range){
	boost::random::uniform_int_distribution<R_t> _dist(range._lowerBound, range._upperBound);
	return _dist(_generator);
}

template<typename R_t>
template<size_t _Bit_size>
inline const R_t
_Random_integer_generator<R_t>::generateNumber() {
	boost::random::independent_bits_engine<stdGenerator_t, _Bit_size, R_t> _engine(_generator);
	return _engine();
}
template<typename R_t>
inline _Random_integer_generator<R_t>::_Random_integer_generator() {
	_generator = stdGenerator_t(std::chrono::steady_clock::now().time_since_epoch().count());
}
template<typename R_t>
inline _Random_integer_generator<R_t>::_Random_integer_generator(const unsigned long long& seed) : _generator(stdGenerator_t(seed)) {}
template<typename R_t>
inline _Random_integer_generator<R_t>::_Random_integer_generator(const stdGenerator_t _gen) : _generator(_gen) {}

#endif // !_RANDOM_INTEGER_H