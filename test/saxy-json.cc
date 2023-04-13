#include <catch2/catch.hpp>
#include "../include/saxy-json.hpp"
#include <iostream>

using namespace saxy_json;

TEST_CASE("Writer", "[write]")
{
    Writer writer{std::cout};

    writer.StartObject(); // Root object

    writer.Key("name");
    writer.String("John Doe");

    writer.Key("age");
    writer.Int(35);

    writer.Key("address");
    writer.StartObject(); // Address object

    writer.Key("street");
    writer.String("123 Main St");

    writer.Key("city");
    writer.String("Springfield");

    writer.Key("state");
    writer.String("IL");

    writer.Key("zip_code");
    writer.String("62704");

    writer.FinishObject(); // End of address object

    writer.Key("phone_numbers");
    writer.StartArray(); // Phone numbers array

    writer.StartObject(); // First phone number object

    writer.Key("type");
    writer.String("home");

    writer.Key("number");
    writer.String("555-1234");

    writer.FinishObject(); // End of first phone number object

    writer.StartObject(); // Second phone number object

    writer.Key("type");
    writer.String("work");

    writer.Key("number");
    writer.String("555-5678");

    writer.FinishObject(); // End of second phone number object

    writer.FinishArray(); // End of phone numbers array

    writer.Key("email");
    writer.String("john.doe@example.com");

    writer.FinishObject(); // End of root object

    std::cout << std::endl;
}

TEST_CASE("PrettyWriter", "[pretty_write]")
{
    PrettyWriter writer{std::cout};

    writer.StartObject(); // Root object

    writer.Key("name");
    writer.String("John Doe");

    writer.Key("age");
    writer.Int(35);

    writer.Key("address");
    writer.StartObject(); // Address object

    writer.Key("street");
    writer.String("123 Main St");

    writer.Key("city");
    writer.String("Springfield");

    writer.Key("state");
    writer.String("IL");

    writer.Key("zip_code");
    writer.String("62704");

    writer.FinishObject(); // End of address object

    writer.Key("phone_numbers");
    writer.StartArray(); // Phone numbers array

    writer.StartObject(); // First phone number object

    writer.Key("type");
    writer.String("home");

    writer.Key("number");
    writer.String("555-1234");

    writer.FinishObject(); // End of first phone number object

    writer.StartObject(); // Second phone number object

    writer.Key("type");
    writer.String("work");

    writer.Key("number");
    writer.String("555-5678");

    writer.FinishObject(); // End of second phone number object

    writer.FinishArray(); // End of phone numbers array

    writer.Key("email");
    writer.String("john.doe@example.com");

    writer.FinishObject(); // End of root object

    std::cout << std::endl;
}

TEST_CASE("Reader", "[read]")
{
}

