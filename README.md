# RSA-encryption-project

An end-to-end implementation of RSA encryption that includes: full implementation of BigInt class with all math and comparison operators, Sieve of Eratosthenes & Miller–Rabin algorithms fot primality test, implementation of Modular exponentiation & Extended Euclidean algorithm, developing algorithm to convert between messages and big ints and random engine for generate digits.

## Application features
  1. Generate random BigInt with number of bits as your choice.
  2. Primality test for BigInt.
  3. Generate RSA encryption key pair with number of bits as your choice.
  4. Encrypt message using public RSA key.
  5. Decrypt cypher code using private RSA key.

## Estimated time to generate RSA keys
   * 32   bits - about 3 seconds
   * 64   bits - about 25 seconds
   * 128  bits - about 
   * 256  bits - about 10 minutes
   * 512  bits - about
   * 1024 bits - about
   * 2048 bits - about
   * 4096 bits - about
  
## Requirements
   c++ compiler
  
## Run the application   
   * go to directory - RSA-encryption-project/src
   * run command:
```bash
   make check
```

## RSA Algorithm Example

* Choose p = 3 and q = 11
* Compute n = p * q = 3 * 11 = 33
* Compute φ(n) = (p - 1) * (q - 1) = 2 * 10 = 20
* Choose e such that 1 < e < φ(n) and e and φ (n) are coprime. Let e = 7
* Compute a value for d such that (d * e) % φ(n) = 1. One solution is d = 3 [(3 * 7) % 20 = 1]
* Public key is (e, n) => (7, 33)
* Private key is (d, n) => (3, 33)
* The encryption of m = 2 is c = 27 % 33 = 29
* The decryption of c = 29 is m = 293 % 33 = 2

## License
This project is licensed under the GNU General Public License v3.0 - see the LICENSE.md file for details
