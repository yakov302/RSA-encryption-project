#include "../inc/rsa.hpp"

namespace rsa
{

extern bool stop;

std::random_device seed;    
static std::mt19937_64 seed_engine(seed());
std::uniform_int_distribution<int> distribution_int;
std::uniform_int_distribution<> distribution(0, 9);

bool prime_number_flag_array[PRIME_NUMBERS_POOL_RANGE];
static bool is_prime_numbers_vector_was_built = false;
std::vector<int> prime_numbers_vector;

namespace impl
{

void sieve_of_eratosthenes_generator()
{
    prime_numbers_vector.reserve(PRIME_NUMBERS_POOL_RANGE/2);
    memset(prime_number_flag_array, true, sizeof(prime_number_flag_array));
 
    for(int p = 2; (p * p) <= PRIME_NUMBERS_POOL_RANGE; ++p) 
    {
        if (prime_number_flag_array[p] == true) 
        {
            for (int i = p * p; i <= PRIME_NUMBERS_POOL_RANGE; i += p)
                prime_number_flag_array[i] = false;
        }
    }

    for(int i = 2; i < PRIME_NUMBERS_POOL_RANGE; ++i)
    {
        if(prime_number_flag_array[i])
            prime_numbers_vector.push_back(i);
    }
}

bool primality_test(BigInt& big_int)
{
    if(!is_prime_numbers_vector_was_built)
    {
        is_prime_numbers_vector_was_built = true;
        sieve_of_eratosthenes_generator();
    }

    for(auto digit : prime_numbers_vector)
    {
        //relevant only for numbers smaller than PRIME_NUMBERS_POOL_RANGE
        // if(BigInt(digit) == big_int) 
        //     return true;

        if((big_int%digit) == "0")
            return false;   
    }

    return true;
}

//return (a^m)%big_int
BigInt modular_exponentiation(BigInt a, BigInt m, BigInt& big_int) 
{
    BigInt result = 1;      
    a = a%big_int;                  
                                
    while (m > "0")
    {
        if (m.is_i_am_odd())
            result = (result*a)%big_int;

        m = m/2;
        a = (a*a)%big_int;
    }
    
    return result; 
}
 
BigInt rand(BigInt& max)
{
    std::string random;
    int size = distribution_int(seed_engine)%(max.num_of_digits());
    if(size < 2){size = 2;}
        
    for(int i = 0; i < size; ++i)
        random.push_back(distribution(seed_engine) + '0');

    return BigInt(random)%max;
}

bool miller_rabin_iteration(BigInt& big_int, BigInt& m, BigInt& k)
{
    BigInt big_int_minus_1 = big_int - 1;
    BigInt a = rand(big_int);
    BigInt result = modular_exponentiation(a, m, big_int);

    if (result == 1 || result == big_int_minus_1)
       return true;
 
    for(BigInt i = "0"; i < k; ++i)
    {
        result = (result*result)%big_int; 
        if (result == 1)                return false;
        if (result == big_int_minus_1)  return true;
    }
 
    return false;
}
 
bool miller_rabin_test(BigInt& big_int, int iterations)
{ 
    BigInt m = big_int - 1;
    BigInt k = "0";
    while(!m.is_i_am_odd())
    {
        m /= 2;
        ++k;
    }
 
    for(int i = 0; i < iterations; ++i)
        if(!miller_rabin_iteration(big_int, m, k))
            return false;
 
    return true;
}

BigInt extended_euclidean(BigInt& phi, BigInt& e) 
{
    BigInt prev;
    BigInt i = 1;
    BigInt res = ((phi*i) + 1);
    BigInt d = res%e;

    while(d != "0")
    {
        prev = d;
        res = ((phi*i) + 1);
        d = res%e;

        if(prev == d )
            return "0";

        ++i; 
    }

    return res/e;
}

int set_num_of_ints(int message_length)
{
    int num_of_int = message_length/sizeof(int);
    if(message_length%sizeof(int) != 0) 
        num_of_int += 1;
    return num_of_int;
}

void enter_num_of_digits_in_the_int(std::string& temp_string, std::string& string)
{
    if(temp_string.size() < 10)
        string += "0";
    string += std::to_string(temp_string.size());
}

BigInt set_finel_big_int(std::string& string)
{
    BigInt big_int(string);
    if(string[0] == '0')
        big_int.append_on_left(0);
    big_int.append_on_left(1);
    return big_int;
}

void enter_chars(int& char_index, int& message_length, std::string& string, char* message)
{
    for(char i = 0; i < message_length; ++i)
    {
        std::string temp_string = std::to_string(*(char*)(message + char_index + i));
        enter_num_of_digits_in_the_int(temp_string, string);
        string += temp_string;
    }
}

void enter_ints(int& char_index, std::string& string, char* message)
{
    std::string temp_string = std::to_string(*(int*)(message + char_index));
    enter_num_of_digits_in_the_int(temp_string, string);
    string += temp_string;
}

BigInt convert_message_to_big_int(char* message)
{
    int message_length = strlen(message);
    int num_of_int = set_num_of_ints(message_length);

    std::string string;
    string.reserve(message_length + num_of_int*2);

    int char_index = 0;
    for(int i = 0; i < num_of_int; ++i)
    {
        if(message_length < 3)
            enter_chars(char_index, message_length, string, message);
        else
            enter_ints(char_index, string, message);

        message_length -= 4;
        char_index += sizeof(int);
    }

    return set_finel_big_int(string);
}

int set_iteration_size(std::string string_number, int char_insex)
{
    int iteration_size;
    iteration_size = (string_number[char_insex] - '0')*10;
    iteration_size += (string_number[char_insex + 1] - '0');
    return iteration_size;
}

void convert_big_int_to_message(BigInt& big_int, std::string& message)
{
    int message_size = big_int.num_of_digits();
    int* ints = (int*)calloc(message_size, sizeof(int));
    std::string string_number = big_int.convert_big_int_to_string();

    int int_index = 0;
    int iteration_size;
    for (int char_insex = 1; char_insex < message_size; char_insex += iteration_size + 2)
    {
        int power_index = 1;
        iteration_size = set_iteration_size(string_number, char_insex);

        for(int digit_index = char_insex + 2; digit_index < iteration_size + char_insex + 2; ++digit_index)
        {
            ints[int_index] += (string_number[digit_index] - '0')*pow(10, iteration_size - power_index);
            ++power_index;
        }

        ++int_index;
    }

    int num_of_chars = int_index*sizeof(int);
    for (char i = 0; i < num_of_chars; ++i)
        message += *(char*)((char*)ints + i);
    
    free(ints);
}


}//impl namespace


BigInt random_big_int(int num_of_bits)
{
    std::string random;
    int num_of_digits = num_of_bits*log10(2);
    for(int i = 0; i < num_of_digits; ++i)
        random.push_back(distribution(seed_engine) + '0');

    return BigInt(random);
}

bool is_big_int_prime(BigInt& big_int)
{
    if(!big_int.is_i_am_odd())
        return false;

    if(!impl::primality_test(big_int))
        return false;

    if(!impl::miller_rabin_test(big_int, NUM_OF_MILLER_RABIN_TEST_ITERATIONS))
        return false;

    return true;
}

bool is_big_int_prime(BigInt&& big_int)
{
    return is_big_int_prime(big_int);
}

void generate_keys(int num_of_bits)
{
    BigInt n;
    BigInt d;
    BigInt e = 7;
    BigInt p = "4";
    BigInt q = "4";
    BigInt phi = "0";
    int num_of_bits_for_p_and_q = num_of_bits/2;

    while(phi <= "0" || d == "0")
    {
        while(!is_big_int_prime(p))
            p = random_big_int(num_of_bits_for_p_and_q);

        while(!is_big_int_prime(q))
            q = random_big_int(num_of_bits_for_p_and_q);

        n = p * q;
        phi = (p - 1)*(q - 1);

        d = impl::extended_euclidean(phi, e);
    }

    stop = true;
    usleep(100000);
    std::cout << "\n" << GREEN; 
    std::cout << "Public key \n" << "e: " << e << "\nn: " << n << "\n";
    std::cout << "\nPrivate key \n" << "d: " << d << "\nn: " << n << "\n";
}

BigInt encrypt(BigInt& e, BigInt& n, char* message)
{
    BigInt number_message = impl::convert_message_to_big_int(message);
    if(number_message.num_of_digits() > n.num_of_digits())
    {
        std::cout << RED << "\nRSA ERROR: message too long for key (n)" << NORMAL << "\n";
        return BigInt();
    }
    
    return impl::modular_exponentiation(number_message, e, n);
}

std::string decrypt(BigInt& d, BigInt& n, BigInt& c)
{
    std::string message;
    BigInt number_message = impl::modular_exponentiation(c, d, n);
    impl::convert_big_int_to_message(number_message, message);
    return message;
}


} //rsa namespace