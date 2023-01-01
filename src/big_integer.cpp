#include "../inc/big_integer.hpp"

namespace big_integer
{

Compare less([](int first, int second){return (first < second);});
Compare greater([](int first, int second){return (first > second);});

namespace impl
{

bool sign(long long int number)
{
	if(number < 0)
	 	return NEGATIVE;
	return POSITIVE;
}

bool sign(char sign)
{
	if(sign == '-')
	 	return NEGATIVE;
	return POSITIVE;
}

void convet_int_to_list_of_digit(BigIntVector& digits, long long int number)
{
	if(number == 0)
	{
		digits.push_back(0);
		return;
	}

	if(number < 0)
		number *= -1;

	while(number > 0)
	{
		char digit = number % 10;
		number = (number - digit) / 10;
		digits.push_back(digit);
	}
}

void convert_string_to_list_of_digit(BigIntVector& big_int, std::string const& number)
{
	const int num_of_digit = number.size();
	for (int i = num_of_digit - 1; i >= 0; i--)
	{
		if(isdigit(number[i]))
			big_int.push_back((number[i] - '0'));
	}
}

void convert_chars_to_list_of_digit(BigIntVector& big_int, const char* number, int len)
{
	for (int i = len - 1; i >= 0; i--)
	{
		if(isdigit(number[i]))
			big_int.push_back((number[i] - '0'));
	}
}

void check_tail_add(BigIntVector& result, BigIntVector& big_int, int& index, int& index_end, int& carry)
{
	while (index < index_end)
	{
		int current_digits_add = big_int[index] + carry;
		result.push_back(current_digits_add%10);
		carry = current_digits_add/10;
		index++;
	}
}

int sub_iteration_calculation(int first_digits, int second_digits, int& carry)
{
	int current_digits_sub = first_digits - second_digits - carry;
	if(current_digits_sub < 0)
	{
		carry = 1;
		current_digits_sub += 10;
		return current_digits_sub;
	}

	carry = 0;
	return current_digits_sub;
}

void check_tail_sub(BigIntVector& first, int& first_i, int& first_len, BigIntVector& result, int& carry)
{
	while (first_i < first_len)
	{
		int current_digits_sub = sub_iteration_calculation(first[first_i], 0, carry);
		result.push_back(current_digits_sub);
		first_i++;
	}
}

bool is_i_am_zero(BigIntVector& big_int)
{
	int count = 0;
	int len = big_int.size();
	for(int i = len - 1; i >= 0; --i)
	{
		count += (int)big_int[i];
		if(count != 0)
			return false;
	}

	return true;
}

void delete_zeros_on_left(BigIntVector& result)
{
	int len = result.size() - 1;
	for(int i = len; i >=0; --i)
	{
		if(result[i] == 0)
			result.pop_back();
		else
			break;
	}

	if(result.size() == 0)
		result.push_back(0);
}

int absolute_values_comparison(BigIntVector first, BigIntVector second, Compare& compare)
{
	delete_zeros_on_left(first);
	delete_zeros_on_left(second);

	if(compare(first.size(), second.size()))
		return FIRST;

	if(compare(second.size(), first.size()))
		return SECOND;

	int first_i = first.size() -1;
	int second_i = second.size() -1;
	while(first_i >= 0)
	{
		if(compare(first[first_i], second[second_i]))
			return FIRST;

		if(compare(second[second_i], first[first_i]))
			return SECOND;

		--first_i;
		--second_i;
	}

	return EQUALS;
}

int compare(BigIntVector& first, bool& first_sign, BigIntVector& second, bool& second_sign)
{
	if(first_sign != second_sign)
	{
		if(first_sign == POSITIVE)
			return FIRST;
		else
			return SECOND;
	}

	if(first_sign == POSITIVE)
		return absolute_values_comparison(first, second, greater);
	else
		return absolute_values_comparison(first, second, less);
}

bool set_first_and_second(BigIntVector& first, bool& first_sign, BigIntVector& second, bool& second_sign, BigIntVector& self, bool& self_sign, BigIntVector& right_side, bool& right_side_sign, int& first_len, int&  second_len)
{
	int result = absolute_values_comparison(self, right_side, greater);
	if(result == EQUALS)
		return false;

	if(result == FIRST)
	{
		first = self;
		first_sign = self_sign;
		first_len = self.size();

		second = right_side;
		second_sign = right_side_sign;
		second_len = right_side.size();
	}

	if(result == SECOND)
	{
		first = right_side;
		first_sign = right_side_sign;
		first_len = right_side.size();

		second = self;
		second_sign = self_sign;
		second_len = self.size();
	}

	return true;
}

void set_sub_sign(bool& result_sign, bool& self_sign, bool& right_side_sign, BigIntVector& self, BigIntVector& right_side)
{
	int result = absolute_values_comparison(self, right_side, greater);
	if(result == FIRST)
	{
		if(self_sign == NEGATIVE && right_side_sign == NEGATIVE)
			result_sign = NEGATIVE;

		return;
	}

	if(result == SECOND)
	{
		if(self_sign == POSITIVE && right_side_sign == POSITIVE)
			result_sign = NEGATIVE;

		return;
	}
}

void set_mul_sign(bool& result_sign, bool& self_sign, bool& right_side_sign)
{
	if(self_sign != right_side_sign)
		result_sign = NEGATIVE;
	else
		result_sign = POSITIVE;
}

void set_dev_sign(bool& result_sign, bool& self_sign, bool& right_side_sign)
{
	set_mul_sign(result_sign, self_sign, right_side_sign);
	right_side_sign = POSITIVE;
	self_sign = POSITIVE;
}

void set_mod_sign(bool& self_sign, bool& right_side_sign)
{
	self_sign = POSITIVE;
	right_side_sign = POSITIVE;
}

void set_sign_back(bool& self, bool& right_side, bool self_sign, bool right_side_sign)
{
	self = self_sign;
	right_side = right_side_sign;
}

void set_mod_sign_back(bool& self, bool& right_side, bool& result, bool self_sign, bool right_side_sign)
{
	set_sign_back(self, right_side, self_sign, right_side_sign);
	result = self_sign;
}

void flip_sign(bool& big_int_sign)
{
	if(big_int_sign == NEGATIVE)
		big_int_sign = POSITIVE;
	else
		big_int_sign = NEGATIVE;
}

void zeros_padding(BigIntVector& big_int, int& padding_index)
{
	for(int i = 0; i < padding_index; ++i)
		big_int.push_back(0);
}

void close_mul_iteration(BigInt& temp, BigIntVector& temp_vector, BigInt& result, int& second_i, int& carry, int& padding_index)
{
	if (carry != 0)
	{
		temp_vector.push_back(carry);
		carry = 0;
	}

	result = result + temp;
	temp_vector.clear();
	padding_index += 1;
	second_i++;
}

BigInt make_sub(BigInt& self, bool& self_sign, BigInt& right_side)
{
	impl::flip_sign(self_sign);
	BigInt result = self - right_side;
	impl::flip_sign(self_sign);
	impl::flip_sign(result.my_sign());
	return result;
}

BigInt make_add(BigInt& self, bool& self_sign, BigInt& right_side)
{
	impl::flip_sign(self_sign);
	BigInt result = self + right_side;
	impl::flip_sign(self_sign);
	impl::flip_sign(result.my_sign());
	return result;
}

bool one_of_them_is_zero(BigIntVector& self, BigIntVector& right_side)
{
	if(is_i_am_zero(self) || is_i_am_zero(right_side))
		return true;

	return false;
}

bool dev_special_cases(BigIntVector& self, bool& self_sign, BigIntVector& right_side, bool& right_side_sign, BigInt& result)
{
	if(is_i_am_zero(right_side))
		throw std::runtime_error(std::string("ERROR: invalid division by zero!"));

	int resoult = absolute_values_comparison(self, right_side, greater);

	if(resoult == SECOND)
	{
		result = BigInt("0");
		return true;
	}

	if(resoult == EQUALS)
	{
		if(self_sign == right_side_sign)
			result = BigInt("1");
		else
			result = BigInt("-1");
		return true;
	}
		
	return false;
}

bool special_cases_mod(BigInt& self, BigInt& right_side, BigInt& result)
{
	if(right_side  ==  BigInt("0"))
		throw std::runtime_error(std::string("ERROR: invalid division by zero!"));

	if(self < right_side)
	{
		result = self;
		return true;
	}

	if(self == right_side)
	{
		result = BigInt("0");
		return true;
	}

	return false;
}

bool special_cases_pow(bool right_side_sign, BigInt& result, BigInt& self, BigInt& right_side)
{
	if(self == BigInt("1"))
	{
		result = BigInt("1");
		return true;
	}

	if(self == BigInt("-1"))
	{
		result = BigInt("-1");
		return true;
	}

	if(right_side_sign == NEGATIVE)
	{
		if(self == BigInt("0"))
			throw std::runtime_error(std::string("ERROR: invalid division by zero!"));
		
		result = BigInt("0");
		return true;
	}
	
	if(right_side == BigInt("0"))
	{
		result = BigInt("1");
		return true;
	}
	
	if(right_side == BigInt("1"))
	{
		result = self;
		return true;
	}

	return false;
}

bool special_cases_root(BigInt& self)
{
	if(self < BigInt("0"))
		throw std::runtime_error(std::string("ERROR: invalid negative number!"));

	if(self == BigInt("0") || self == BigInt("1"))
		return true;
	
	return false;
}

bool divide_temp_self_by_right_side(BigInt& temp_self, BigInt& right_side, BigInt& temp_result, BigInt& temp_result_mul_right_side, bool first_iteration)
{
	if(right_side > temp_self && first_iteration)
		return false;

	temp_result = "0";
	temp_result_mul_right_side = "0";
	while(true)
	{
		temp_result_mul_right_side = temp_result_mul_right_side + right_side;
		if(temp_result_mul_right_side > temp_self)
			break;
		else
			temp_result = temp_result + 1;
	}

	temp_result_mul_right_side = temp_result_mul_right_side - right_side;
	return true;
}

void push_front(BigIntVector& big_int, int digit)
{
	int len = big_int.size();
	big_int.push_back(0);
	for(int i = len - 1; i >= 0; --i)
		big_int[i+1] = big_int[i];
	 
	big_int[0] = digit;
}

void concatenate(BigIntVector& result, BigIntVector& temp_result)
{
	for(auto digit : temp_result)
		push_front(result, digit);
}

bool is_i_am_root(BigInt& root, BigInt& number, BigInt& upper, BigInt& lower)
{
	BigInt root_pow_2 = root*root;
	if(root_pow_2 > number)
	{
		upper = root;
		return false;
	}

	if(root_pow_2 < number)
	{
		lower = root;
		return false;
	}

	return true;
}


}// impl namespace

BigInt::BigInt()
: sign(POSITIVE)
, digits()
{

}

BigInt::BigInt(long long int number)
: sign(POSITIVE)
, digits()
{
	sign = impl::sign(number);
	digits.reserve(20);
	impl::convet_int_to_list_of_digit(digits, number);
	impl::delete_zeros_on_left(this->digits);
}

BigInt::BigInt(const char* number)
: sign(POSITIVE)
, digits()
{
	sign = impl::sign(number[0]);
	int len = strlen(number);
	digits.reserve(len);
	impl::convert_chars_to_list_of_digit(digits, number, len);
	impl::delete_zeros_on_left(this->digits);
}

BigInt::BigInt(std::string const& number)
: sign(POSITIVE)
, digits()
{
	sign = impl::sign(number[0]);
	digits.reserve(number.size());
	impl::convert_string_to_list_of_digit(digits, number);
	impl::delete_zeros_on_left(this->digits);
}

BigInt::BigInt(std::string const&& number)
: sign(POSITIVE)
, digits()
{
	sign = impl::sign(number[0]);
	digits.reserve(number.size());
	impl::convert_string_to_list_of_digit(digits, number);
	impl::delete_zeros_on_left(this->digits);
}

BigInt::BigInt(BigInt const& source)
: sign(source.sign)
, digits()
{
	digits = source.digits;
}

BigInt::BigInt(BigInt const&& source)
: sign(source.sign)
, digits()
{
	digits = std::move(source.digits);
}

BigInt& BigInt::operator=(BigInt const& right_side)
{
	this->digits = right_side.digits;
	this->sign = right_side.sign;
	return *this;
}

BigInt& BigInt::operator=(BigInt const&& right_side)
{
	this->digits = std::move(right_side.digits);
	this->sign = right_side.sign;
	return *this;
}

BigInt& BigInt::operator=(const char* right_side)
{
	*this = BigInt(right_side);
	return *this;
}

BigInt& BigInt::operator=(std::string const& right_side)
{
	*this = BigInt(right_side);
	return *this;
}

BigInt& BigInt::operator=(std::string const&& right_side)
{
	*this = BigInt(right_side);
	return *this;
}

BigInt& BigInt::operator=(long long int const& right_side)
{
	*this = BigInt(right_side);
	return *this;
}

BigInt BigInt::operator+(BigInt& right_side)
{
	if(this->sign != right_side.sign)
		return impl::make_sub(*this, this->sign, right_side);

	int carry = 0;
	int first_i = 0;
	int second_i = 0;
	int first_len = this->digits.size();
	int second_len = right_side.digits.size();

	BigInt result;
	result.sign = this->sign;
	result.reserve(first_len + second_len);

	while (first_i < first_len && second_i < second_len)
	{
		int current_digits_add = this->digits[first_i] + right_side.digits[second_i] + carry;
		result.digits.push_back(current_digits_add%10);
		carry = current_digits_add/10;
		first_i++;
		second_i++;
	}

	impl::check_tail_add(result.digits, this->digits, first_i, first_len, carry);
	impl::check_tail_add(result.digits, right_side.digits, second_i, second_len, carry);
	if (carry)
		result.digits.push_back(carry);
	return result;
}

BigInt BigInt::operator+(BigInt&& right_side)
{
	return *this + right_side;
}

BigInt BigInt::operator+(const char* right_side)
{
	return *this + BigInt(right_side);
}

BigInt BigInt::operator+(long long int right_side)
{
	return *this + BigInt(right_side);
}

BigInt BigInt::operator+(std::string& right_side)
{
	return *this + BigInt(right_side);
}

BigInt BigInt::operator+(std::string&& right_side)
{
	return *this + BigInt(right_side);
}

void BigInt::sub(BigInt& first, BigInt& second, BigInt& result)
{
	int carry = 0;
	int first_i = 0;
	int second_i = 0;
	int first_len = first.num_of_digits();
	int second_len = second.num_of_digits();
	result.reserve(first_len);

	while (first_i < first_len && second_i < second_len)
	{
		int current_digits_sub = impl::sub_iteration_calculation(first.digits[first_i], second.digits[second_i], carry);
		result.digits.push_back(current_digits_sub);
		first_i++;
		second_i++;
	}

	impl::check_tail_sub(first.digits, first_i, first_len, result.digits, carry);
}

BigInt BigInt::operator-(BigInt& right_side)
{
	if(this->sign != right_side.sign)
		return impl::make_add(*this, this->sign, right_side);

	int compar_result = impl::absolute_values_comparison(this->digits, right_side.digits, greater);
	if(compar_result == EQUALS)
		return BigInt("0");

	BigInt result;
	if(compar_result == FIRST)
		sub(*this, right_side, result);

	if(compar_result == SECOND)
		sub(right_side, *this, result);

	impl::set_sub_sign(result.sign,  this->sign, right_side.sign, this->digits, right_side.digits);
	impl::delete_zeros_on_left(result.digits);
	return result;
}

BigInt BigInt::operator-(BigInt&& right_side)
{
	return *this - right_side;
}

BigInt BigInt::operator-(const char* right_side)
{
	return *this - BigInt(right_side);
}

BigInt BigInt::operator-(long long int right_side)
{
	return *this - BigInt(right_side);
}

BigInt BigInt::operator-(std::string& right_side)
{
	return *this - BigInt(right_side);
}

BigInt BigInt::operator-(std::string&& right_side)
{
	return *this - BigInt(right_side);
}

BigInt BigInt::operator*(BigInt& right_side)
{
	if(impl::one_of_them_is_zero(this->digits, right_side.digits))
		return BigInt("0");

	int carry = 0;
	int second_i = 0;
	int padding_index = 0;
	int first_len = this->digits.size();
	int second_len = right_side.digits.size();

	BigInt temp;
	BigInt result;
	temp.reserve(first_len + 1);
	result.reserve(first_len + second_len);
	

	while (second_i < second_len)
	{
		int first_i = 0;
		impl::zeros_padding(temp.digits, padding_index);

		while(first_i < first_len)
		{
			int current_digits_mul = this->digits[first_i]*right_side.digits[second_i] + carry;
			temp.digits.push_back(current_digits_mul%10);
			carry = current_digits_mul/10;
			first_i++;
		}

		impl::close_mul_iteration(temp, temp.digits, result, second_i, carry, padding_index);
	}

	impl::set_mul_sign(result.sign, this->sign, right_side.sign);
	impl::delete_zeros_on_left(result.digits);
	return result;
}

BigInt BigInt::operator*(BigInt&& right_side)
{
	return (*this) * right_side;
}

BigInt BigInt::operator*(const char* right_side)
{
	return (*this) * BigInt(right_side);
}

BigInt BigInt::operator*(long long int right_side)
{
	return (*this)* BigInt(right_side);
}

BigInt BigInt::operator*(std::string& right_side)
{
	return (*this) * BigInt(right_side);
}

BigInt BigInt::operator*(std::string&& right_side)
{
	return (*this) * BigInt(right_side);
}

BigInt BigInt::operator/(BigInt& right_side)
{
	BigInt result;
	if(impl::dev_special_cases(this->digits, this->sign, right_side.digits, right_side.sign, result))
		return result;

	int first_len = this->digits.size();
	int first_i = first_len - 1;
	bool first_iteration = true;
	bool self_sign = this->sign;
	bool right_side_sign = right_side.sign;

	BigInt temp_self; 
	BigInt temp_result;
	BigInt temp_result_mul_right_side;
	temp_self.reserve(first_len);
	temp_result.reserve(first_len);
	temp_result_mul_right_side.reserve(first_len);

	impl::set_dev_sign(result.sign, this->sign, right_side.sign);

	while(first_i >= 0)
	{
		impl::push_front(temp_self.digits, this->digits[first_i]);
		if(impl::divide_temp_self_by_right_side(temp_self, right_side, temp_result, temp_result_mul_right_side, first_iteration))
		{
			first_iteration = false;
			impl::concatenate(result.digits, temp_result.digits);
			temp_self = temp_self - temp_result_mul_right_side;
		}

		--first_i;
	}

	impl::set_sign_back(this->sign, right_side.sign, self_sign, right_side_sign);
	return result;
}

BigInt BigInt::operator/(BigInt&& right_side)
{
	return *this / right_side;
}

BigInt BigInt::operator/(const char* right_side)
{
	return *this / BigInt(right_side);
}

BigInt BigInt::operator/(long long int right_side)
{
	return *this / BigInt(right_side);
}

BigInt BigInt::operator/(std::string& right_side)
{
	return *this / BigInt(right_side);
}

BigInt BigInt::operator/(std::string&& right_side)
{
	return *this / BigInt(right_side);
}

BigInt BigInt::operator%(BigInt& right_side)
{
	BigInt result;
	bool self_sign = this->sign;
	bool right_side_sign = right_side.sign;
	impl::set_mod_sign(this->sign, right_side.sign);
	if(impl::special_cases_mod(*this, right_side, result))
	{
		impl::set_mod_sign_back(this->sign, right_side.sign, result.sign, self_sign, right_side_sign);
		return result;
	}		

	BigInt dev = *this/right_side;
	result = (*this) - (dev*right_side);

	impl::set_mod_sign_back(this->sign, right_side.sign, result.sign, self_sign, right_side_sign);
	return result;
}

BigInt BigInt::operator%(BigInt&& right_side)
{
	return *this % right_side;
}

BigInt BigInt::operator%(const char* right_side)
{
	return *this % BigInt(right_side);
}

BigInt BigInt::operator%(long long int right_side)
{
	return *this % BigInt(right_side);
}

BigInt BigInt::operator%(std::string& right_side)
{
	return *this % BigInt(right_side);
}

BigInt BigInt::operator%(std::string&& right_side)
{
	return *this % BigInt(right_side);
}

BigInt BigInt::operator^(BigInt& right_side)
{
	BigInt result(1);
	if(impl::special_cases_pow(right_side.sign, result, *this, right_side))
		return result;

	BigInt counter("0");
	while(counter < right_side)
	{
		result = result * (*this);
		counter = counter + 1;
	}

	return result;
}

BigInt BigInt::operator^(BigInt&& right_side)
{
	return *this ^ right_side;
}

BigInt BigInt::operator^(const char* right_side)
{
	return *this ^ BigInt(right_side);
}

BigInt BigInt::operator^(long long int right_side)
{
	return *this ^ BigInt(right_side);
}

BigInt BigInt::operator^(std::string& right_side)
{
	return *this ^ BigInt(right_side);
}

BigInt BigInt::operator^(std::string&& right_side)
{
	return *this ^ BigInt(right_side);
}

BigInt BigInt::square_root()
{
	if(impl::special_cases_root(*this))
		return *this;

    BigInt lower("1");
    BigInt upper = *this;
	BigInt root = *this/2;

	while(upper > (lower + 1))
	{
		if(impl::is_i_am_root(root, *this, upper, lower))
			break;

		root = lower + ((upper - lower)/BigInt(2));
	}

	return root;
}

BigInt BigInt::operator++()
{
	*this = *this + 1;
	return *this;
}

BigInt BigInt::operator++(int)
{
	BigInt pre = *this;
	*this = *this + 1;
	return pre;
}

BigInt BigInt::operator--()
{
	*this = *this - 1;
	return *this;
}

BigInt BigInt::operator--(int)
{
	BigInt pre = *this;
	*this = *this - 1;
	return pre;
}

BigInt BigInt::operator+=(BigInt& right_side)
{
	return *this = *this + right_side;
}

BigInt BigInt::operator+=(BigInt&& right_side)
{
	return *this += right_side;
}

BigInt BigInt::operator+=(const char* right_side)
{
	return *this += BigInt(right_side);
}

BigInt BigInt::operator+=(long long int right_side)
{
	return *this += BigInt(right_side);
}

BigInt BigInt::operator+=(std::string& right_side)
{
	return *this += BigInt(right_side);
}

BigInt BigInt::operator+=(std::string&& right_side)
{
	return *this += BigInt(right_side);
}

BigInt BigInt::operator-=(BigInt& right_side)
{
	return *this = *this - right_side;
}

BigInt BigInt::operator-=(BigInt&& right_side)
{
	return *this -= right_side;
}

BigInt BigInt::operator-=(const char* right_side)
{
	return *this -= BigInt(right_side);
}

BigInt BigInt::operator-=(long long int right_side)
{
	return *this -= BigInt(right_side);
}

BigInt BigInt::operator-=(std::string& right_side)
{
	return *this -= BigInt(right_side);
}

BigInt BigInt::operator-=(std::string&& right_side)
{
	return *this -= BigInt(right_side);
}

BigInt BigInt::operator*=(BigInt& right_side)
{
	return *this = (*this) * right_side;
}

BigInt BigInt::operator*=(BigInt&& right_side)
{
	return *this *= right_side;
}

BigInt BigInt::operator*=(const char* right_side)
{
	return *this *= BigInt(right_side);
}

BigInt BigInt::operator*=(long long int right_side)
{
	return *this *= BigInt(right_side);
}

BigInt BigInt::operator*=(std::string& right_side)
{
	return *this *= BigInt(right_side);
}

BigInt BigInt::operator*=(std::string&& right_side)
{
	return *this *= BigInt(right_side);
}

BigInt BigInt::operator/=(BigInt& right_side)
{
	return *this = (*this) / right_side;
}

BigInt BigInt::operator/=(BigInt&& right_side)
{
	return *this /= right_side;
}

BigInt BigInt::operator/=(const char* right_side)
{
	return *this /= BigInt(right_side);
}

BigInt BigInt::operator/=(long long int right_side)
{
	return *this /= BigInt(right_side);
}

BigInt BigInt::operator/=(std::string& right_side)
{
	return *this /= BigInt(right_side);
}

BigInt BigInt::operator/=(std::string&& right_side)
{
	return *this /= BigInt(right_side);
}

BigInt BigInt::operator%=(BigInt& right_side)
{
	return *this = (*this) % right_side;
}

BigInt BigInt::operator%=(BigInt&& right_side)
{
	return *this %= right_side;
}

BigInt BigInt::operator%=(const char* right_side)
{
	return *this %= BigInt(right_side);
}

BigInt BigInt::operator%=(long long int right_side)
{
	return *this %= BigInt(right_side);
}

BigInt BigInt::operator%=(std::string& right_side)
{
	return *this %= BigInt(right_side);
}

BigInt BigInt::operator%=(std::string&& right_side)
{
	return *this %= BigInt(right_side);
}

BigInt BigInt::operator^=(BigInt& right_side)
{
	return *this = (*this) ^ right_side;
}

BigInt BigInt::operator^=(BigInt&& right_side)
{
	return *this ^= right_side;
}

BigInt BigInt::operator^=(const char* right_side)
{
	return *this ^= BigInt(right_side);
}

BigInt BigInt::operator^=(long long int right_side)
{
	return *this ^= BigInt(right_side);
}

BigInt BigInt::operator^=(std::string& right_side)
{
	return *this ^= BigInt(right_side);
}

BigInt BigInt::operator^=(std::string&& right_side)
{
	return *this ^= BigInt(right_side);
}

bool BigInt::operator==(BigInt& right_side)
{
	if(impl::compare(this->digits, this->sign, right_side.digits, right_side.sign) != EQUALS)
		return false;

	return true;
}

bool BigInt::operator==(BigInt&& right_side)
{
	return *this == right_side;
}

bool BigInt::operator==(const char* right_side)
{
	return *this == BigInt(right_side);
}

bool BigInt::operator==(long long int right_side)
{
	return *this == BigInt(right_side);
}

bool BigInt::operator==(std::string& right_side)
{
	return *this == BigInt(right_side);
}

bool BigInt::operator==(std::string&& right_side)
{
	return *this == BigInt(right_side);
}

bool BigInt::operator!=(BigInt& right_side)
{
	return !(*this == right_side);
}

bool BigInt::operator!=(BigInt&& right_side)
{
	return *this != right_side;
}

bool BigInt::operator!=(const char* right_side)
{
	return *this != BigInt(right_side);
}

bool BigInt::operator!=(long long int right_side)
{
	return *this != BigInt(right_side);
}

bool BigInt::operator!=(std::string& right_side)
{
	return *this != BigInt(right_side);
}

bool BigInt::operator!=(std::string&& right_side)
{
	return *this != BigInt(right_side);
}

bool BigInt::operator<(BigInt& right_side)
{
	if(impl::compare(this->digits, this->sign, right_side.digits, right_side.sign) != SECOND)
		return false;

	return true;
}

bool BigInt::operator<(BigInt&& right_side)
{
	return *this < right_side;
}

bool BigInt::operator<(const char* right_side)
{
	return *this < BigInt(right_side);
}

bool BigInt::operator<(long long int right_side)
{
	return *this < BigInt(right_side);
}

bool BigInt::operator<(std::string& right_side)
{
	return *this < BigInt(right_side);
}

bool BigInt::operator<(std::string&& right_side)
{
	return *this < BigInt(right_side);
}

bool BigInt::operator>(BigInt& right_side)
{
	if(impl::compare(this->digits, this->sign, right_side.digits, right_side.sign) != FIRST)
		return false;

	return true;
}

bool BigInt::operator>(BigInt&& right_side)
{
	return *this > right_side;
}

bool BigInt::operator>(const char* right_side)
{
	return *this > BigInt(right_side);
}

bool BigInt::operator>(long long int right_side)
{
	return *this > BigInt(right_side);
}

bool BigInt::operator>(std::string& right_side)
{
	return *this > BigInt(right_side);
}

bool BigInt::operator>(std::string&& right_side)
{
	return *this > BigInt(right_side);
}

bool BigInt::operator<=(BigInt& right_side)
{
	return (*this < right_side || *this == right_side);
}

bool BigInt::operator<=(BigInt&& right_side)
{
	return *this <= right_side;
}

bool BigInt::operator<=(const char* right_side)
{
	return *this <= BigInt(right_side);
}

bool BigInt::operator<=(long long int right_side)
{
	return *this <= BigInt(right_side);
}

bool BigInt::operator<=(std::string& right_side)
{
	return *this <= BigInt(right_side);
}

bool BigInt::operator<=(std::string&& right_side)
{
	return *this <= BigInt(right_side);
}

bool BigInt::operator>=(BigInt& right_side)
{
	return (*this > right_side || *this == right_side);
}

bool BigInt::operator>=(BigInt&& right_side)
{
	return *this >= right_side;
}

bool BigInt::operator>=(const char* right_side)
{
	return *this >= BigInt(right_side);
}

bool BigInt::operator>=(long long int right_side)
{
	return *this >= BigInt(right_side);
}

bool BigInt::operator>=(std::string& right_side)
{
	return *this >= BigInt(right_side);
}

bool BigInt::operator>=(std::string&& right_side)
{
	return *this >= BigInt(right_side);
}

std::ostream& operator<<(std::ostream& a_os, BigInt const& big_int)
{
	if(big_int.sign == NEGATIVE)
		std::cout << "-";

	int size = big_int.digits.size();
	for(int i = size - 1; i >= 0; --i)
		std::cout << (int)big_int.digits[i];

	return a_os;
}

std::string BigInt::convert_big_int_to_string()
{
	std::string string;
	int len = this->digits.size();
	string.reserve(len);
	
	for(int i = len - 1; i >= 0; --i)
		string.push_back(this->digits[i] + '0');
	return string;
}

long long int BigInt::convert_big_int_to_long_long()
{
	int i = 0;	
	int pos = 0;
	long long int number = 0; 
	int len = this->digits.size();
	while(i < len)
	{
		number += this->digits[i]*(pow(10,pos));
		pos++;
		i++;
	}

	if(this->sign == NEGATIVE)
		number*=-1;
	return number;
}

bool BigInt::is_i_am_odd()
{
	return (this->digits[0]%2) != 0;
}

bool& BigInt::my_sign()
{
	return this->sign;
}

size_t BigInt::num_of_digits()const
{
	return this->digits.size();
}

void BigInt::reserve(int num_of_digit)
{
	this->digits.reserve(num_of_digit);
}

void BigInt::append_on_left(int digit)
{
	this->digits.push_back(digit);
}


} //big_integer namespace