#pragma once

#include <unistd.h>
#include <iostream>
#include <pthread.h>

#include "../inc/rsa.hpp"

namespace rsa
{

#define GENERATE_BIG_INT 1
#define CHECK_IF_BIG_INT_PRIME 2 
#define GENERATE_RSA_KEY_PAIR 3
#define ENCRYPT_MESSAGE 4
#define DECRYPT_CYPHER_CODE 5
#define PROGRESS_BAR_WIDTH 25

void run();


}// namespace rsa
