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

The library includes a type check concept, `Integral`, to ensure that the input type is either an integral type or a Boost multiprecision integer.

### Useful Functions

- **mulMod**: Modular multiplication of two integers.
- **powMod**: Modular exponentiation of an integer.
- **isPrime**: Primality test using the Miller-Rabin algorithm.
- **binToDec**: Convert a binary string to an integer.
- **decToBin**: Convert an integer to a binary string.

### Random Number Generation

- **getRandomInteger**: Generate a random integer within a specified range.
- **getRandomPrime**: Generate a random prime number within a specified range.
- **getRandomBits**: Generate a random integer with a specified number of bits.

### Example Usage


```cpp
#include "RandomInteger.hpp"
#include <iostream>
#include <format>

using boost::multiprecision::cpp_int;

int main(){
    auto start = clock();
    std::cout << getRandomPrime<cpp_int, 4096>() << std::endl;
    auto stop = clock();
    std::cout << std::format("Time taken: {}", (double) (stop - start) / CLOCKS_PER_SEC);
}
```
```
> 267499123177963081456063244039872328066925621977261766562078797643872805044261675953830708369754492678607410135932430306
059040724988105975266923731638689544577341391448953010330397028623851681888822708339448202986682550356523351650774652308
996908597077191738679625289429766686625645677302648267818879850927835214633181820547423575317903146912230243814427773049
513318988429274809781079838278116778342379800773483895210184743619256520697901817685649009412027484002685220984379827751
935779403528313754131577125446456410277111114137689746038568794230161512346335928592752520589650380714363337756782518485
321820005345939332428963073477929630577933757341481178919496054583715570349038536317765268088070688063758408628611775300
709272677751643184652936956420496709700437444849386621091762096670285483421447019564019568849829202477758111209397092263
598963569584486719601124749306612351666103372548671899674154450405354813451570369559122822088414422856906016106369705843
477885747971398250755812269510792476111321695821978861864293971784472563575471623583948682045225991248344908872081765797
869630158609825503550249871235433367740558156687937467053351763391507303775568212213775034477389117002196577983254245610
383601561695294680441912788168087
Time taken: 8.508
```
## Dependencies

- [Boost C++ Libraries](https://www.boost.org/)

## Installation

To use this library, make sure to have the Boost C++ Libraries installed. Include the `"RandomInteger.hpp"` header file in your C++ project.

## License

This library is provided under the [MIT License](LICENSE).

Feel free to contribute or report issues on [GitHub](https://github.com/your_username/random-integer-generator).
