#include "myconverters.h"
#include<string>
#include<sstream>

int atoi(const char* str) {
    int num = 0;
    char digit;
    while ((digit = *str++) != '\0') {
	if (digit < '0' || digit > '9') {
	    return num; /* No valid conversion possible */
	}
	num *= 10;
	num += digit - '0';
    }
    return num;
}

char* itoa(int i) {
    std::stringstream ss;
    std::string out;
    ss << i;
    ss >> out;
    return (char*) out.c_str();
}