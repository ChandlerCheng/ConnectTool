#include <string>
#include <iostream>
#include <fstream>
#include <atlstr.h>
#include <vector>


void appendToVector(std::vector<unsigned char>& vec, const char* str);

void appendToVector(std::vector<unsigned char>& vec, unsigned char* str, int size);

void appendToVector(std::vector<unsigned char>& vec, const std::string& str);

void appendToVector(std::vector<unsigned char>& vec, std::vector<unsigned char>& vec2);

std::vector<unsigned char> convertToCodePage(const std::vector<unsigned char>& input, UINT codePage);



