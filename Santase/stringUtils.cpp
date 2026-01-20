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
* <String algorithms>
*
*/

#include "stringUtils.h"
#include "errorHandler.h"

void split(char* str, const char splitter) {
	if (str == nullptr) {
		errorHandler(ERROR::error_0);
		return;
	} 

	while (*str != '\0') {
		if (*str == splitter) *str = '\0';
		str++;
	}
}

void strCopy(char* target, const char* src) {
	if (target == nullptr || src == nullptr) {
		errorHandler(ERROR::error_0);
	}

	while (*target != '\0' && *src != '\0') {
		*target = *src;
		target++;
		src++;
	}
	*target = '\0';
}

bool strCompare(const char* str1, const char* str2) {
	if (str1 == nullptr || str2 == nullptr) {
		errorHandler(ERROR::error_0);
		return false;
	}

	while (*str1 != '\0' && *str2 != '\0') {
		if (*str1 != *str2) return false;
		str1++;
		str2++;
	}

	return *str1 == *str2;
}

int convertStrToNum(const char* str) {
	int num = 0;
	if (*str == '\0') {
		errorHandler(ERROR::error_1);
		return -1;
	}
	while (*str != '\0') {
		if (*str < '0' || *str > '9') {
			errorHandler(ERROR::error_1);
			return -1;
		}
		num = num * 10 + (*str - '0');
		str++;
	}
	return num;
}