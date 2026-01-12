#include "errorHandler.h"
#include<iostream>

void errorHandler(const ERROR error) {
    if (error == ERROR::error_0) {
        std::cout << "Nullptr Error!" << std::endl;
    }
    else if (error == ERROR::error_1) {
        std::cout << "Incorrect input please write a correct one" << std::endl;
    }
    else if (error == ERROR::error_2) {
        std::cout << "Invalid argument given" << std::endl;
    }
}
