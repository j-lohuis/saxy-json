#ifndef INCLUDED_SAXY_JSON_HPP
#define INCLUDED_SAXY_JSON_HPP

#include <iostream>

#include <cstdint>
#include <exception>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <ostream>
#include <limits>
#include <concepts>
#include <charconv>
#include <functional>

namespace saxy_json
{
template<typename OStream = std::ostream>
class Writer
{
public:
    explicit Writer(OStream& ostream)
        : ostream(ostream)
    {
        level_stack.push_back(false);
    }

    virtual void StartObject()
    {
        MaybeComma();
        level_stack.emplace_back(false);
        Put('{');
    }
    virtual void FinishObject()
    {
        level_stack.pop_back();
        Put('}');
    }
    virtual void StartArray()
    {
        MaybeComma();
        level_stack.push_back(false);
        Put('[');
    }
    virtual void FinishArray()
    {
        level_stack.pop_back();
        Put(']');
    }
    virtual void Key(std::string_view key)
    {
        MaybeComma();
        Put('"');
        WriteEscaped(key);
        Put('"');
        Put(':');
        level_stack.back() = false;
    }
    
    void KeyValue(std::string_view key, std::integral auto value)
    {
        Key(key);
        Int(value);
    }

    void KeyValue(std::string_view key, std::floating_point auto value)
    {
        Key(key);
        Float(value);
    }

    void KeyValue(std::string_view key, bool value)
    {
        Key(key);
        Bool(value);
    }

    void KeyValue(std::string_view key, std::nullptr_t)
    {
        Key(key);
        Null();
    }

    void KeyValue(std::string_view key, std::string_view value)
    {
        Key(key);
        String(value);
    }

    void String(std::string_view str)
    {
        Put('"');
        WriteEscaped(str);
        Put('"');
        level_stack.back() = true;
    }
    void Bool(bool b)
    {
        if (b)
            RawString("true");
        else
            RawString("false");
        level_stack.back() = true;
    }
    void Int(std::integral auto i)
    {
        std::array<char, std::numeric_limits<decltype(i)>::digits10> buffer;

        if (auto [ptr, ec] =
                std::to_chars(buffer.data(), buffer.data() + buffer.size(), i);
            ec == std::errc())
            RawString(buffer.data());
        else
            std::terminate();

        level_stack.back() = true;
    }
    void Float(std::floating_point auto f)
    {
        std::array<char, std::numeric_limits<decltype(f)>::max_digits10> buffer;

        if (auto [ptr, ec] =
                std::to_chars(buffer.data(), buffer.data() + buffer.size(), f);
            ec == std::errc())
            RawString(buffer.data());
        else
            std::terminate();

        level_stack.back() = true;
    }
    void Null()
    {
        RawString("null");
        level_stack.back() = true;
    }
    void RawString(std::string_view str)
    {
        for (char c : str)
            Put(c);
    }

protected:
    void WriteEscaped(std::string_view str)
    {
        for (char c : str)
        {
            switch (c)
            {
            case '"': RawString("\\\""); break;
            case '\\': RawString("\\\\"); break;
            case '\b': RawString("\\b"); break;
            case '\f': RawString("\\f"); break;
            case '\n': RawString("\\n"); break;
            case '\r': RawString("\\r"); break;
            case '\t': RawString("\\t"); break;
            default: Put(c); break;
            }
        }
    }

    void MaybeComma()
    {
        if (level_stack.back())
            Put(',');
        else
            level_stack.back() = true;
    }

    void Put(char c)
    {
        if constexpr (requires { ostream.put(c); })
            ostream.put(c);
        else
            ostream << c;
    }


    OStream& ostream;
    std::vector<bool> level_stack;
};

template<>
inline void Writer<std::string>::Put(char c)
{
    ostream.push_back(c);
}

template<typename OStream = std::ostream>
class PrettyWriter : public Writer<OStream>
{
public:
    explicit PrettyWriter(OStream& ostream, int indent = 4)
        : Writer<OStream>(ostream)
        , indent_size(indent)
        , current_indent(0)
    {
        in_array_stack.push_back(false);
    }

    void StartObject() override
    {
        this->MaybeComma();
        if (in_array_stack.back())
        {
            this->Put('\n');
            WriteIndent();
        }
        this->Put('{');
        current_indent += indent_size;
        in_array_stack.push_back(false);
        this->level_stack.push_back(false);
    }
    void FinishObject() override
    {
        current_indent -= indent_size;
        this->level_stack.pop_back();
        in_array_stack.pop_back();
        this->Put('\n');
        WriteIndent();
        this->Put('}');
    }
    void StartArray() override
    {
        this->MaybeComma();
        if (in_array_stack.back())
        {
            this->Put('\n');
            WriteIndent();
        }
        this->Put('[');
        current_indent += indent_size;
        in_array_stack.push_back(true);
        this->level_stack.push_back(false);
    }
    void FinishArray() override
    {
        current_indent -= indent_size;
        this->level_stack.pop_back();
        in_array_stack.pop_back();
        this->Put('\n');
        WriteIndent();
        this->Put(']');
    }
    void Key(std::string_view key) override
    {
        this->MaybeComma();
        this->Put('\n');
        WriteIndent();
        this->Put('"');
        this->WriteEscaped(key);
        this->Put('"');
        this->Put(':');
        this->Put(' ');
        this->level_stack.back() = false;
    }

protected:
    void WriteIndent()
    {
        for (int i = 0; i < current_indent; ++i)
            this->Put(' ');
    }

    int indent_size;
    int current_indent;
    std::vector<bool> in_array_stack;
};

struct Handler
{
    void (*StartObject)();
    void (*FinishObject)();
    void (*StartArray)();
    void (*FinishArray)();
    void (*Key)(std::string&& key);
    void (*String)(std::string&& val);
    void (*Bool)(bool val);
    void (*Int)(std::intmax_t val);
    void (*Float)(double val);
    void (*Null)();
    void (*Error)(std::string&& msg);
};

namespace detail
{
using namespace std::string_literals;
/// json
///     element
template<typename THandler, typename IStream>
void ParseJson(THandler& handler, IStream& istream);

/// value
///     object
///     array
///     string
///     number
///     "true"
///     "false"
///     "null"
template<typename THandler, typename IStream>
void ParseValue(THandler& handler, IStream& istream);

/// object
///     '{' ws '}'
///     '{' members '}'
template<typename THandler, typename IStream>
void ParseObject(THandler& handler, IStream& istream);

/// members
///     member
///     member ',' members
template<typename THandler, typename IStream>
void ParseMembers(THandler& handler, IStream& istream);

/// member
///     ws string ws ':' element
template<typename THandler, typename IStream>
void ParseMember(THandler& handler, IStream& istream);

/// array
///     '[' ws ']'
///     '[' elements ']'
template<typename THandler, typename IStream>
void ParseArray(THandler& handler, IStream& istream);

/// elements
///     element
///     element ',' elements
template<typename THandler, typename IStream>
void ParseElements(THandler& handler, IStream& istream);

/// element
///     ws value ws
template<typename THandler, typename IStream>
void ParseElement(THandler& handler, IStream& istream);

/// string
///     '"' characters '"'
template<typename THandler, typename IStream>
void ParseString(THandler& handler, IStream& istream);

/// number
///     integer fraction exponent
template<typename THandler, typename IStream>
void ParseNumber(THandler& handler, IStream& istream);

/// bool-literal
///     "true"
///     "false"
template<typename THandler, typename IStream>
void ParseBoolLiteral(THandler& handler, IStream& istream);

/// null
template<typename THandler, typename IStream>
void ParseNull(THandler& handler, IStream& istream);

template<typename THandler>
void AddToString(std::string& str, int ch, THandler& handler)
{
    if (ch == -1)
        handler.Error("Unexpected EOF"s);

    str += static_cast<char>(ch);
}

template<typename THandler, typename IStream>
void SkipWhitespace([[maybe_unused]] THandler& handler, IStream& istream)
{
    auto ch = istream.peek();
    while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
    {
        istream.get();
        ch = istream.peek();
    }
}

template<typename THandler, typename IStream>
void Expect(int c, THandler& handler, IStream& istream)
{
    int ch = istream.get();
    if (ch != c)
        handler.Error("Unexpected char '"s + static_cast<char>(ch) + "'"s);
}

template<typename THandler, typename IStream>
std::string GetEscapedString(THandler& handler, IStream& istream)
{
    Expect('"', handler, istream);
    std::string str;
    auto ch = istream.get();
    while (ch != '"')
    {
        if (ch == '\\')
        {
            ch = istream.get();
            switch (ch)
            {
            case '"': str += '"'; break;
            case '\\': str += '\\'; break;
            case '/': str += '/'; break;
            case 'b': str += '\b'; break;
            case 'f': str += '\f'; break;
            case 'n': str += '\n'; break;
            case 'r': str += '\r'; break;
            case 't': str += '\t'; break;
            case 'u':
            {
                std::array<char, 2> first;
                std::array<char, 2> second;

                if (!isxdigit(istream.peek()))
                    handler.Error("Invalid hex character '"s + static_cast<char>(istream.peek()) + "'"s);
                first[0] = static_cast<char>(istream.peek());
                istream.get();

                if (!isxdigit(istream.peek()))
                    handler.Error("Invalid hex character '"s + static_cast<char>(istream.peek()) + "'"s);
                first[1] = static_cast<char>(istream.peek());
                istream.get();

                if (!isxdigit(istream.peek()))
                    handler.Error("Invalid hex character '"s + static_cast<char>(istream.peek()) + "'"s);
                second[0] = static_cast<char>(istream.peek());
                istream.get();

                if (!isxdigit(istream.peek()))
                    handler.Error("Invalid hex character '"s + static_cast<char>(istream.peek()) + "'"s);
                second[1] = static_cast<char>(istream.peek());

                uint8_t res;
                std::from_chars(first.data(), first.data() + first.size(), res, 16);
                str += static_cast<char>(res);
                std::from_chars(second.data(), second.data() + second.size(), res, 16);
                str += static_cast<char>(res);
            }
            break;
            default:
                handler.Error("Invalid escape character '"s + static_cast<char>(ch) + "'"s);
            }
        }
        else
        {
            AddToString(str, ch, handler);
        }
        ch = istream.get();
    }
    return str;
}

template<typename THandler, typename IStream>
void ParseJson(THandler& handler, IStream& istream)
{
    ParseElement(handler, istream);
}

template<typename THandler, typename IStream>
void ParseValue(THandler& handler, IStream& istream)
{
    switch (istream.peek())
    {
        case '{': ParseObject(handler, istream); break;
        case '[': ParseArray(handler, istream); break;
        case '"': ParseString(handler, istream); break;
        case '-': [[fallthrough]];
        case '0': [[fallthrough]];
        case '1': [[fallthrough]];
        case '2': [[fallthrough]];
        case '3': [[fallthrough]];
        case '4': [[fallthrough]];
        case '5': [[fallthrough]];
        case '6': [[fallthrough]];
        case '7': [[fallthrough]];
        case '8': [[fallthrough]];
        case '9': ParseNumber(handler, istream); break;
        case 't': [[fallthrough]];
        case 'f': ParseBoolLiteral(handler, istream); break;
        case 'n': ParseNull(handler, istream); break;
        default: handler.Error("Unexpected character '"s + static_cast<char>(istream.peek()) + "' in ParseValue"s);
    }
}

template<typename THandler, typename IStream>
void ParseObject(THandler& handler, IStream& istream)
{
    handler.StartObject();
    istream.get();

    ParseMembers(handler, istream);

    Expect('}', handler, istream);
    handler.FinishObject();
}

template<typename THandler, typename IStream>
void ParseMembers(THandler& handler, IStream& istream)
{
    ParseMember(handler, istream);
    while (istream.peek() == ',')
    {
        istream.get();
        ParseMember(handler, istream);
    }
}

template<typename THandler, typename IStream>
void ParseMember(THandler& handler, IStream& istream)
{
    SkipWhitespace(handler, istream);

    if (istream.peek() == '}')
        return;

    auto key = GetEscapedString(handler, istream);
    handler.Key(std::move(key));
    SkipWhitespace(handler, istream);
    Expect(':', handler, istream);
    ParseElement(handler, istream);
}

template<typename THandler, typename IStream>
void ParseArray(THandler& handler, IStream& istream)
{
    istream.get();
    handler.StartArray();
    ParseElements(handler, istream);
    Expect(']', handler, istream);
    handler.FinishArray();
}

template<typename THandler, typename IStream>
void ParseElements(THandler& handler, IStream& istream)
{
    SkipWhitespace(handler, istream);

    if (istream.peek() == ']')
        return;

    ParseElement(handler, istream);

    while (istream.peek() == ',')
    {
        istream.get();
        ParseElement(handler, istream);
    }
}

/// element
///     ws value ws
template<typename THandler, typename IStream>
void ParseElement(THandler& handler, IStream& istream)
{
    SkipWhitespace(handler, istream);
    ParseValue(handler, istream);
    SkipWhitespace(handler, istream);
}

/// string
///     '"' characters '"'
template<typename THandler, typename IStream>
void ParseString(THandler& handler, IStream& istream)
{
    auto str = GetEscapedString(handler, istream);
    handler.String(std::move(str));
}

/// number
///     integer fraction exponent
template<typename THandler, typename IStream>
void ParseNumber(THandler& handler, IStream& istream)
{
    std::string number;
    if (istream.peek() == '-')
        AddToString(number, istream.get(), handler);

    while (std::isdigit(istream.peek()))
        AddToString(number, istream.get(), handler);

    if (istream.peek() != '.' && istream.peek() != 'e' && istream.peek() != 'E')
    {
        // Integer
        std::intmax_t integer;
        const auto& [ptr, ec] = std::from_chars(number.data(), number.data() + number.size(), integer);
        if (ec == std::errc{})
            handler.Int(integer);
        else
            handler.Error("Could not convert '"s + number + "' to integer"s);
    }
    else
    {
        istream.get();
        auto ch = istream.peek();
        while (std::isdigit(ch) || ch == 'e' || ch == 'E' || ch == '+' || ch == '-')
        {
            AddToString(number, istream.get(), handler);
            ch = istream.peek();
        }
        double floating_point;
        const auto& [ptr, ec] = std::from_chars(number.data(), number.data() + number.size(), floating_point);
        if (ec == std::errc{})
            handler.Float(floating_point);
        else
            handler.Error("Could not convert '"s + number + "' to float"s);
    }
}

template<typename THandler, typename IStream>
void ParseBoolLiteral(THandler& handler, IStream& istream)
{
    if (istream.peek() == 't')
    {
        Expect('t', handler, istream);
        Expect('r', handler, istream);
        Expect('u', handler, istream);
        Expect('e', handler, istream);
        handler.Bool(true);
    }
    else if (istream.peek() == 'f')
    {
        Expect('f', handler, istream);
        Expect('a', handler, istream);
        Expect('l', handler, istream);
        Expect('s', handler, istream);
        Expect('e', handler, istream);
        handler.Bool(false);
    }
}

template<typename THandler, typename IStream>
void ParseNull(THandler& handler, IStream& istream)
{
    Expect('n', handler, istream);
    Expect('u', handler, istream);
    Expect('l', handler, istream);
    Expect('l', handler, istream);
    handler.Null();
}

}

template<typename THandler, typename IStream = std::istream>
void Parse(THandler& handler, IStream& istream)
{
    detail::ParseJson(handler, istream);
}

} // namespace saxy_json

#endif
