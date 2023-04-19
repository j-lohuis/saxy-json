#include <catch2/catch.hpp>
#include "../include/saxy-json.hpp"
#include <iostream>
#include <stdexcept>
#include <string_view>

using namespace saxy_json;

template<typename Writer>
static void build_object(Writer& writer)
{
    writer.StartObject(); // Root object

    writer.KeyValue("name", "John Doe");

    writer.KeyValue("age", 35);

    writer.Key("address");
    writer.StartObject(); // Address object

    writer.KeyValue("street", "123 Main St");
    writer.KeyValue("city", "Springfield");
    writer.KeyValue("state", "IL");
    writer.KeyValue("zip_code", "62704");

    writer.FinishObject(); // End of address object

    writer.Key("phone_numbers");
    writer.StartArray(); // Phone numbers array

    writer.StartObject(); // First phone number object

    writer.KeyValue("type", "home");
    writer.KeyValue("number", "555-5678");

    writer.FinishObject(); // End of first phone number object

    writer.StartObject(); // Second phone number object

    writer.KeyValue("type", "work");
    writer.KeyValue("number", "555-8765");

    writer.FinishObject(); // End of second phone number object

    writer.FinishArray(); // End of phone numbers array

    writer.KeyValue("email", "john.doe@eample.com");

    writer.FinishObject(); // End of root object
}

TEST_CASE("Writer", "[write]")
{
    Writer writer{std::cout};

    build_object(writer);

    std::cout << std::endl;
}

TEST_CASE("PrettyWriter", "[pretty_write]")
{
    std::string res;
    PrettyWriter writer{res};

    build_object(writer);

    std::cout << res << std::endl;
}

class CustomHandler
{
public:
    void StartObject() { std::cout << "StartObject()" << std::endl; }
    void FinishObject() { std::cout << "FinishObject()" << std::endl; }
    void StartArray() { std::cout << "StartArray()" << std::endl; }
    void FinishArray() { std::cout << "FinishArray()" << std::endl; }
    void Key(std::string key)
    {
        std::cout << "Key(" << key << ")" << std::endl;
    }
    void String(std::string val)
    {
        std::cout << "String(" << val << ")" << std::endl;
    }
    void Bool(bool val)
    {
        std::cout << "Bool(" << std::boolalpha << val << ")" << std::endl;
    }
    void Int(intmax_t val) { std::cout << "Int(" << val << ")" << std::endl; }
    void Float(double val) { std::cout << "Float(" << val << ")" << std::endl; }
    void Null() { std::cout << "Null()" << std::endl; }
    void Error(std::string_view msg)
    {
        std::cout << "ERROR: " << msg << std::endl;
    }
};

TEST_CASE("Reader", "[read]")
{
    std::string json_str =
        R"({
            "name": "John Doe",
            "age": 30,
            "is_student": false,
            "courses": ["math", "history", "chemistry"],
            "scores": {"math": 90.5, "history": 88, "chemistry": 79}
        })";
    //    std::string json_str =
    //        R"({"name":"John Doe","age":35,"address":{"street":"123 Main
    //        St","city":"Springfield","state":"IL","zip_code":"62704"},"phone_numbers":[{"type":"home","number":"555-1234"},{"type":"work","number":"555-5678"}],"email":"john.doe@example.com"})";

    std::cout << json_str << std::endl;
    std::istringstream json_stream(json_str);

    CustomHandler handler;
    Parse(handler, json_stream);
}

class TestHandler
{
public:
    void StartObject() { events.push_back("StartObject"); }
    void FinishObject() { events.push_back("FinishObject"); }
    void StartArray() { events.push_back("StartArray"); }
    void FinishArray() { events.push_back("FinishArray"); }
    void Key(std::string&& key) { events.push_back("Key:" + key); }
    void String(std::string&& str) { events.push_back("String:" + str); }
    void Int(std::intmax_t i) { events.push_back("Int:" + std::to_string(i)); }
    void Float(double d) { events.push_back("Float:" + std::to_string(d)); }
    void Bool(bool b) { events.push_back("Bool:" + std::to_string(b)); }
    void Null() { events.push_back("Null"); }
    void Error(std::string&& msg)
    {
        throw std::runtime_error(std::move(msg));
    }

    std::vector<std::string> events;
};

TEST_CASE("JSON parser")
{
    SECTION("Valid JSON")
    {
        std::vector<std::string> tests = {
            R"({"key": "value"})",
            R"(["item1", "item2", 42, 3.14, true, false, null])",
            R"({"key": ["item1", "item2", 42, 3.14, true, false, null]})",
            R"({"key": {"nested_key": "nested_value"}})",
            R"({"escaped\":key": "escaped_value"})",
        };

        for (const auto& test : tests)
        {
            TestHandler handler;
            std::istringstream input(test);
            Parse(handler, input);

            REQUIRE(handler.events.size() > 0);
            REQUIRE(handler.events[0] != "Error");
        }
    }

    SECTION("Invalid JSON")
    {
        std::vector<std::string> tests = {
            R"({key: "value"})",
            R"(["item1", "item2" 42, 3.14, true, false, null])",
            R"({"key" "value"})",
            R"({"key": ["item1", "item2", 42, 3.14, true, false, null]])",
            R"({"key": {"nested_key": "nested_value"})",
            R"({"escaped\kkey": "escaped_value"})",
        };

        for (const auto& test : tests)
        {
            TestHandler handler;
            std::istringstream input(test);
            REQUIRE_THROWS(Parse(handler, input));
        }
    }
}
