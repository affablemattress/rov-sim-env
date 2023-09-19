#include "util.h"

#include <iostream>


int main(int argc, char** args) {
    std::cout << "Hello World!, from main." << std::endl;
    printCString("Hello World!, from extern");
}