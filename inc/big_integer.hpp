#pragma once

#include <vector>
#include <string>
#include <math.h>
#include <cstring>
#include <cstdint>
#include <utility>
#include <iostream>
#include <functional>

namespace big_integer
{

#define POSITIVE true
#define NEGATIVE false
#define FIRST 1
#define SECOND 2
#define EQUALS 3

using  BigIntVector = std::vector<char>;
using  Compare = std::function<bool(int, int)>;

class BigInt
{
public:
    BigInt();
    BigInt(const char* number);
    BigInt(long long int number);
    BigInt(std::string const& number);
    BigInt(std::string const&& number);

    BigInt(BigInt const& source);
    BigInt(BigInt const&& source);

    BigInt& operator=(const char* right_side);
    BigInt& operator=(BigInt const& right_side);
    BigInt& operator=(BigInt const&& right_side);
    BigInt& operator=(std::string const& right_side);
    BigInt& operator=(std::string const&& right_side);
    BigInt& operator=(long long int const& right_side);

    BigInt operator+(const char* right_side);
    BigInt operator+(long long int right_side);
    BigInt operator+(std::string& right_side);
    BigInt operator+(std::string&& right_side);
    BigInt operator+(BigInt& right_side);
    BigInt operator+(BigInt&& right_side);

    BigInt operator-(const char* right_side);
    BigInt operator-(long long int right_side);
    BigInt operator-(std::string& right_side);
    BigInt operator-(std::string&& right_side);
    BigInt operator-(BigInt& right_side);
    BigInt operator-(BigInt&& right_side);

    BigInt operator*(const char* right_side);
    BigInt operator*(long long int right_side);
    BigInt operator*(std::string& right_side);
    BigInt operator*(std::string&& right_side);
    BigInt operator*(BigInt& right_side);
    BigInt operator*(BigInt&& right_side);

    BigInt operator/(const char* right_side);
    BigInt operator/(long long int right_side);
    BigInt operator/(std::string& right_side);
    BigInt operator/(std::string&& right_side);
    BigInt operator/(BigInt& right_side);
    BigInt operator/(BigInt&& right_side);

    BigInt operator%(const char* right_side);
    BigInt operator%(long long int right_side);
    BigInt operator%(std::string& right_side);
    BigInt operator%(std::string&& right_side);
    BigInt operator%(BigInt& right_side);
    BigInt operator%(BigInt&& right_side);

    BigInt operator^(const char* right_side);
    BigInt operator^(long long int right_side);
    BigInt operator^(std::string& right_side);
    BigInt operator^(std::string&& right_side);
    BigInt operator^(BigInt& right_side);  
    BigInt operator^(BigInt&& right_side);

    BigInt square_root();

    BigInt operator++();
    BigInt operator--();  
    BigInt operator++(int); 
    BigInt operator--(int);  

    BigInt operator+=(const char* right_side);
    BigInt operator+=(long long int right_side);
    BigInt operator+=(std::string& right_side);
    BigInt operator+=(std::string&& right_side);
    BigInt operator+=(BigInt& right_side);
    BigInt operator+=(BigInt&& right_side);

    BigInt operator-=(const char* right_side);
    BigInt operator-=(long long int right_side);
    BigInt operator-=(std::string& right_side);
    BigInt operator-=(std::string&& right_side);
    BigInt operator-=(BigInt& right_side);
    BigInt operator-=(BigInt&& right_side);

    BigInt operator*=(const char* right_side);
    BigInt operator*=(long long int right_side);
    BigInt operator*=(std::string& right_side);
    BigInt operator*=(std::string&& right_side);
    BigInt operator*=(BigInt& right_side);
    BigInt operator*=(BigInt&& right_side);

    BigInt operator/=(const char* right_side);
    BigInt operator/=(long long int right_side);
    BigInt operator/=(std::string& right_side);
    BigInt operator/=(std::string&& right_side);
    BigInt operator/=(BigInt& right_side);
    BigInt operator/=(BigInt&& right_side);

    BigInt operator%=(const char* right_side);
    BigInt operator%=(long long int right_side);
    BigInt operator%=(std::string& right_side);
    BigInt operator%=(std::string&& right_side);
    BigInt operator%=(BigInt& right_side);
    BigInt operator%=(BigInt&& right_side);

    BigInt operator^=(const char* right_side);
    BigInt operator^=(long long int right_side);
    BigInt operator^=(std::string& right_side);
    BigInt operator^=(std::string&& right_side);
    BigInt operator^=(BigInt& right_side);
    BigInt operator^=(BigInt&& right_side);

    bool operator==(const char* right_side);
    bool operator==(long long int right_side);
    bool operator==(std::string& right_side);
    bool operator==(std::string&& right_side);
    bool operator==(BigInt& right_side);
    bool operator==(BigInt&& right_side);

    bool operator!=(const char* right_side);
    bool operator!=(long long int right_side);
    bool operator!=(std::string& right_side);
    bool operator!=(std::string&& right_side);
    bool operator!=(BigInt& right_side);
    bool operator!=(BigInt&& right_side);

    bool operator<(const char* right_side);
    bool operator<(long long int right_side);
    bool operator<(std::string& right_side);
    bool operator<(std::string&& right_side);
    bool operator<(BigInt& right_side);
    bool operator<(BigInt&& right_side);

    bool operator>(const char* right_side);
    bool operator>(long long int right_side);
    bool operator>(std::string& right_side);
    bool operator>(std::string&& right_side);
    bool operator>(BigInt& right_side);
    bool operator>(BigInt&& right_side);

    bool operator<=(const char* right_side);
    bool operator<=(long long int right_side);
    bool operator<=(std::string& right_side);
    bool operator<=(std::string&& right_side);
    bool operator<=(BigInt& right_side);
    bool operator<=(BigInt&& right_side);

    bool operator>=(const char* right_side);
    bool operator>=(long long int right_side);
    bool operator>=(std::string& right_side);
    bool operator>=(std::string&& right_side);
    bool operator>=(BigInt& right_side);
    bool operator>=(BigInt&& right_side);
    
    bool& my_sign();
    bool is_i_am_odd();
    size_t num_of_digits()const;
    void append_on_left(int digit);
    void reserve(int num_of_digit);
    std::string convert_big_int_to_string();
    friend std::ostream& operator<<(std::ostream& a_os, BigInt const& big_int);
    long long int convert_big_int_to_long_long(); // Warning!!! use only for numbers smaller than sizeof(long long int)

private:
    void sub(BigInt& first, BigInt& second, BigInt& result);

private:
    bool sign;
    BigIntVector digits;
};


} //namespace big_integer
