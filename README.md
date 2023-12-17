# Random Integer Generator Library

## Overview

This C++ library provides functionality for generating random integers and prime numbers within specified ranges. It utilizes the Boost C++ libraries for efficient random number generation and primality testing.

## Features

- Generation of random integers within a specified range.
- Generation of random prime numbers within a specified range.
- Primality testing using the Miller-Rabin algorithm.
- Flexible API for generating numbers with a custom bit size.

## Usage

### Prerequisites

Ensure that you have the Boost C++ libraries installed to use this library. You can find the Boost libraries [here](https://www.boost.org/).

### Integration

Include the necessary headers in your C++ code:

```c++
#include "boost/multiprecision/miller_rabin.hpp"
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/random.hpp"
#include <iterator>
#include <optional>
#include <chrono>
#include <list>
#include "RandomInteger.h"
```

### Basic Usage

#### Instantiation

```c++
Generator<Integer> generator; // Default constructor with a seed based on the current time.
```

#### Generating Random Integers

```cpp
Integer randomInteger = generator.generateNumber();
```

#### Generating Random Integers within a Range

```cpp
generateRange<Integer> range(1, 100);
Integer randomIntegerInRange = generator.generateNumber(range);
```

#### Generating Random Prime Numbers

```cpp
Integer randomPrime = generator.generatePrime();
```

#### Generating Random Prime Numbers within a Range

```cpp
generateRange<Integer> primeRange(100, 1000);
Integer randomPrimeInRange = generator.generatePrime(primeRange);
```

### Advanced Usage

#### Custom Seed

```cpp
Generator<Integer> customSeedGenerator(123456789); // Custom seed value
```

#### Custom Bit Size

```cpp
Integer customBitSize = generator.generateNumber<128>(); // Generate a 128-bit random integer
Integer customBitSizePrime = generator.generatePrime<256>(); // Generate a 256-bit random prime
```

## Notes

- The library uses the Miller-Rabin primality test with a default of 20 iterations.
- Ensure Boost C++ libraries are properly installed and linked in your project.

## License

This library is provided under the [MIT License](LICENSE). Feel free to use, modify, and distribute it as needed. If you find any issues or have suggestions, please create an [issue](https://github.com/yourusername/yourlibrary/issues) on GitHub.
