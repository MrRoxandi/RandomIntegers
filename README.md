# Random Integer Generator

This C++ library provides functions for generating random integers, with a focus on generating random prime numbers. It uses the Boost C++ libraries for multiprecision arithmetic and Miller-Rabin primality testing.

## Features

- Generate random integers of specified length.
- Generate random prime numbers.
- Modular arithmetic functions (`mulmod`, `powmod`).
- Primality testing using the Miller-Rabin algorithm.

## Usage
```cpp
auto var_1 = primes::getRandomPrime(bit_count) // Produce a prime number with exact bits
auto var_2 = integers::getRandomInteger(100, 50000) // Produce a number in range from 100 to 50000
```
### Type Check

The library includes a type check concept, `Integral`, to ensure that the input type is either an integral type or a Boost multiprecision integer.

### Useful Functions

- **mulMod**: Modular multiplication of two integers.
- **powMod**: Modular exponentiation of an integer.
- **isPrime**: Primality test using the Miller-Rabin algorithm.

### Random Number Generation

- **getRandomInteger**: Generate a random integer within a specified range.
- **getRandomPrime**: Generate a random prime number within a specified range.
- **getRandomBits**: Generate a random integer with a specified number of bits.

### Example Usage
```cpp
#include <iostream>
#include <format>
#include "src/RadInt.hpp"

int main() {
    auto start = clock();
    std::cout << primes::getRandomPrime(1024) << std::endl;
    auto stop = clock();
    std::cout << std::format("Time taken: {}", (double)(stop - start) / CLOCKS_PER_SEC);
}
```
```
> 13825063694961209240409894928962964358370527171457353592699308278169140735521294595301628121516
5446947571369285164476601054567559838052892833450980297209223065547791489731600707371672006864162
1945650129086372620221275998694710857381206397888183876626024558389869743999933157782234045966950
05559434551007265751
Time taken: 0.397
```
## Dependencies

- [Boost C++ Libraries](https://www.boost.org/)

## Installation

To use this library, make sure to have the Boost C++ Libraries installed. Include the `"RandomInteger.hpp"` header file in your C++ project.

## License

This library is provided under the [MIT License](LICENSE).

Feel free to contribute or report issues on [GitHub](https://github.com/your_username/random-integer-generator).
