#include "../inc/app.hpp"

namespace rsa
{

bool stop;

namespace impl
{

void draw(int i, int pos)
{
    if (i < pos) std::cout << "-";
    else if (i == pos) std::cout;
    else std::cout << " ";
}

void back_to_beginning()
{
    std::cout << "\r";
    std::cout.flush();
    usleep(50000);
}

void set_progress(float& progress)
{
    progress += 0.05; 
    if(progress  > 1)
        progress = 0;
}

void delete_bar()
{
    std::cout << "\r";
    std::cout.flush();
    for (int i = 0; i < PROGRESS_BAR_WIDTH*2; ++i) 
        std::cout << " ";
}

void* progress_bar(void*)
{
    float progress = 0;

    while (!stop) 
    {
        std::cout << YELLOW << "RSA processing ";
        int pos = PROGRESS_BAR_WIDTH * progress;

        for (int i = 0; i < PROGRESS_BAR_WIDTH; ++i) 
            draw(i, pos);

        back_to_beginning();
        set_progress(progress);
    }

    delete_bar();
    return 0;
}

void draw_progress_bar()
{
    stop = false;
    pthread_t thread;
    std::cout << "\n";
    pthread_create(&thread, 0, progress_bar, 0);
}

void delete_progress_bar()
{
    stop = true;
    usleep(100000);
}

void print_str_and_big_int(std::string message, BigInt big_int)
{
    std::cout << "\n" << GREEN; 
    std::cout << message << big_int << "\n" << NORMAL;
}

void menu()
{
    std::cout << YELLOW << "\n-------------RSA MENU-------------\n\n" << NORMAL;
    std::cout <<" 1 - Generate big int" << "\n";
    std::cout <<" 2 - Check if big int is prime" << "\n";
    std::cout <<" 3 - Generate RSA key pair"  << "\n";
    std::cout <<" 4 - Encrypt message"  << "\n";
    std::cout <<" 5 - Decrypt cypher code"  << "\n";
	std::cout <<"\nEnter your choice: ";
}

void generate_big_int()
{
    int num_of_bits;
    std::cout << "Select number of bits: ";
    std::cin >> num_of_bits;

    draw_progress_bar();
    BigInt big_int = random_big_int(num_of_bits);
    delete_progress_bar();

    print_str_and_big_int("Big int: ", big_int);
}

void check_if_big_int_is_prime()
{
    std::string string;
    std::cout << "Enter big int: ";
    std::cin >> string;

    draw_progress_bar();
    bool res = is_big_int_prime(BigInt(string));
    delete_progress_bar();

    if(res)
        print_str_and_big_int("Big int is prime", BigInt());
    else
        print_str_and_big_int("Big int is not prime", BigInt());
}

void generate_rsa_key_pair()
{
    int num_of_bits;
    std::cout << "Select number of bits: ";
    std::cin >> num_of_bits;

    draw_progress_bar();
    generate_keys(num_of_bits);
}

void encrypt_message()
{
    std::string message;
    std::cout << "Enter message: ";
    std::cin >> message;
    char* m = (char*)message.c_str();

    std::string e_string;
    std::cout << "Enter e from public key: ";
    std::cin >> e_string;
    BigInt e = e_string;

    std::string n_string;
    std::cout << "Enter n from public key: ";
    std::cin >> n_string;
    BigInt n = n_string;

    draw_progress_bar();
    BigInt c = encrypt(e, n, m);
    delete_progress_bar();

    if(c.num_of_digits() > 0)
        print_str_and_big_int("Cypher code: ", c);
}

void decrypt_cypher_code()
{
    std::string cypher_code;
    std::cout << "Enter Cypher code: ";
    std::cin >> cypher_code;
    BigInt c = cypher_code;

    std::string d_string;
    std::cout << "Enter d from private key: ";
    std::cin >> d_string;
    BigInt d = d_string;

    std::string n_string;
    std::cout << "Enter n from private key: ";
    std::cin >> n_string;
    BigInt n = n_string;

    draw_progress_bar();
    std::string message = "Message: ";
    message += decrypt(d, n, c);
    delete_progress_bar();

    print_str_and_big_int(message, BigInt());
}

void action(int choice)
{
    switch (choice)
    {
    case GENERATE_BIG_INT:
        generate_big_int();
        break;

    case CHECK_IF_BIG_INT_PRIME:
        check_if_big_int_is_prime();
        break;

    case GENERATE_RSA_KEY_PAIR:
        generate_rsa_key_pair();
        break;

    case ENCRYPT_MESSAGE:
        encrypt_message();
        break;

    case DECRYPT_CYPHER_CODE:
        decrypt_cypher_code();
        break; 

    default:
        std::cout << RED << "\nRSA ERROR: Invalid choice" << NORMAL << "\n";
        break;
    }
}


}//impl namespace

void run()
{
    int choice;
    while(true)
    {
        impl::menu();
        std::cin >> choice;
        impl::action(choice);
    }
}


}//rsa namespace