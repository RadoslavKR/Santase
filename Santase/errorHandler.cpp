/**
*
* Solution to course project # <2>
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author <Radoslav Kolev Rachkov>
* @idnumber <6MI0600634>
* @compiler <VC>
*
* <Handels error messages for the use and for future programmers>
*
*/

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
