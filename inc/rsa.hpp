#pragma once

#include <random>
#include <vector>
#include <unistd.h>

#include "../inc/big_integer.hpp"

#define NUM_OF_MILLER_RABIN_TEST_ITERATIONS 10
#define PRIME_NUMBERS_POOL_RANGE 1000
#define YELLOW "\x1B[33m"
#define NORMAL "\x1B[0m"
#define GREEN "\x1B[32m"
#define RED "\x1B[91m"

using namespace big_integer;

namespace rsa
{

BigInt random_big_int(int num_of_bits);
bool is_big_int_prime(BigInt& big_int);
bool is_big_int_prime(BigInt&& big_int);

void generate_keys(int num_of_bits);
BigInt encrypt(BigInt& e, BigInt& n, char* m);
std::string decrypt(BigInt& d, BigInt& n, BigInt& c);


} //rsa namespace