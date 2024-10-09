#include "vectorTool.h"

#pragma region VECTOR_APPEND
void appendToVector(std::vector<unsigned char>& vec, const char* str) {
	vec.insert(vec.end(), str, str + strlen(str));  // 插入 const char* 字符串
}
void appendToVector(std::vector<unsigned char>& vec, unsigned char* str, int size)
{
	vec.insert(vec.end(), str, str + size);  // 插入 const char* 字符串
}
void appendToVector(std::vector<unsigned char>& vec, const std::string& str) {
	vec.insert(vec.end(), str.begin(), str.end());  // 插入 std::string 字符串
}
void appendToVector(std::vector<unsigned char>& vec, std::vector<unsigned char>& vec2) {
	vec.insert(vec.end(), vec2.begin(), vec2.end());  // 插入 std::string 字符串
}
#pragma endregion


std::string HexStr2ASCII(const std::string& hexStr)
{
    std::string asciiStr;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        std::string byteString = hexStr.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteString, nullptr, 16));
        asciiStr += byte;
    }
    return asciiStr;
}
std::wstring utf8ToWstring(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string wstringToEncoding(const std::wstring& wstr, UINT codePage) {
    int size_needed = WideCharToMultiByte(codePage, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(codePage, 0, wstr.c_str(), (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::string utf8ToBig5(const std::string& utf8Str) {
    std::wstring wstr = utf8ToWstring(utf8Str);
    return wstringToEncoding(wstr, 950);  // 950 is the code page for Big-5
}

std::string utf8ToGBK(const std::string& utf8Str) {
    std::wstring wstr = utf8ToWstring(utf8Str);
    return wstringToEncoding(wstr, 936);  // 936 is the code page for GBK
}

// 将 UTF-8 编码的字符串转换为 GB2312 编码
std::string utf8ToGB2312(const std::string& utf8Str) {
    std::wstring wstr = utf8ToWstring(utf8Str);
    return wstringToEncoding(wstr, 20936);  // 20936 is the code page for GB2312
}
// 将 std::string 或者 std::vector<unsigned char> 转换为目标代码页的多字节字符串
std::vector<unsigned char> convertToCodePage(const std::vector<unsigned char>& input, UINT codePage) 
{
    // 将输入的 std::vector<unsigned char> 转换为 std::string
    std::string inputStr(input.begin(), input.end());

    // 进行编码转换
    std::string convertedStr;
    if (codePage == 65001) {  // UTF-8
        convertedStr = inputStr;  // 已经是 UTF-8，不需要转换
    }
    else if (codePage == 950) {  // Big-5
        convertedStr = utf8ToBig5(inputStr);
    }
    else if (codePage == 936) {  // GBK
        convertedStr = utf8ToGBK(inputStr);
    }
    else if (codePage == 20936) {  // GB2312
        convertedStr = utf8ToGB2312(inputStr);
    }
    else {
        // 默认处理为 UTF-8 或抛出异常/错误处理
        throw std::runtime_error("Unsupported code page");
    }

    // 将转换后的 std::string 转换回 std::vector<unsigned char>
    return std::vector<unsigned char>(convertedStr.begin(), convertedStr.end());
}