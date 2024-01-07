# Random Integer Generator

This C++ library provides functions for generating random integers, with a focus on generating random prime numbers. It uses the Boost C++ libraries for multiprecision arithmetic and Miller-Rabin primality testing.

## Features

- Generate random integers of specified length.
- Generate random prime numbers.
- Modular arithmetic functions (`mulmod`, `powmod`).
- Primality testing using the Miller-Rabin algorithm.
- Conversion functions between binary and decimal representations.

## Usage

### Type Check

The library includes a type check struct, `isInteger`, to ensure that the input type is either an integral type or a Boost multiprecision integer.

### Useful Functions

- **mulmod**: Modular multiplication of two integers.
- **powmod**: Modular exponentiation of an integer.
- **isPrime**: Primality test using the Miller-Rabin algorithm.
- **abs**: Absolute value of an integer.
- **binToDec**: Convert a binary string to an integer.
- **decToBin**: Convert an integer to a binary string.
- **bitLen**: Calculate the number of bits required to represent an integer.

### Random Number Generation

- **getRandomInteger**: Generate a random integer within a specified range.
- **getRandomPrime**: Generate a random prime number within a specified range.
- **getRandomBits**: Generate a random integer with a specified number of bits.

### Example Usage

```cpp
#include "random_integer.h"

int main() {
    // Generate a random prime number between 100 and 1000
    auto randomPrime = getRandomPrime<uint32_t>(100, 1000);
    
    // Perform modular exponentiation
    auto result = powmod<uint64_t>(2, 10, 100);
    
    return 0;
}
```

## Dependencies

- [Boost C++ Libraries](https://www.boost.org/)

## Installation

To use this library, make sure to have the Boost C++ Libraries installed. Include the `"random_integer.h"` header file in your C++ project.

## License

This library is provided under the [MIT License](LICENSE).

Feel free to contribute or report issues on [GitHub](https://github.com/your_username/random-integer-generator).
