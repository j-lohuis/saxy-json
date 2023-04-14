#include "../include/saxy-json.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

class TestHandler
{
public:
    void StartObject() {}
    void FinishObject() {}
    void StartArray() {}
    void FinishArray() {}
    void Key(std::string) {}
    void String(std::string) {}
    void Int(std::intmax_t) {}
    void Float(double) {}
    void Bool(bool) {}
    void Null() {}
    void Error(std::string&& msg)
    {
        throw std::runtime_error(std::move(msg));
    }
};

int main(int argc, const char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " INFILE" << std::endl;
    }

    auto file = std::ifstream(argv[1], std::ios::in);
    auto handler = TestHandler();
    std::cout << argv[1] << ';';
    try
    {
        saxy_json::Parse(handler, file);
        std::cout << "Accepted" << std::endl;
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "Failed with " << err.what() << std::endl;
    }
}

