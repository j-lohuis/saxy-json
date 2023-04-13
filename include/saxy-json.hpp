#ifndef INCLUDED_SAXY_JSON_HPP
#define INCLUDED_SAXY_JSON_HPP

#include <string>
#include <string_view>
#include <vector>
#include <ostream>

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
        ostream.put('{');
    }
    virtual void FinishObject()
    {
        level_stack.pop_back();
        ostream.put('}');
    }
    virtual void StartArray()
    {
        MaybeComma();
        level_stack.push_back(false);
        ostream.put('[');
    }
    virtual void FinishArray()
    {
        level_stack.pop_back();
        ostream.put(']');
    }
    virtual void Key(std::string_view key)
    {
        MaybeComma();
        ostream.put('"');
        WriteEscaped(key);
        ostream.put('"');
        ostream.put(':');
        level_stack.back() = false;
    }

    void String(std::string_view str)
    {
        ostream.put('"');
        WriteEscaped(str);
        ostream.put('"');
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
        ostream << i;
        level_stack.back() = true;
    }
    void Float(std::floating_point auto f)
    {
        ostream << f;
        level_stack.back() = true;
    }
    void Null()
    {
        RawString("null");
        level_stack.back() = true;
    }
    void RawString(std::string_view str)
    {
        ostream << str;
    }

protected:
    void WriteEscaped(std::string_view str)
    {
        for (char c : str)
        {
            switch (c)
            {
                case '"': ostream << "\\\""; break;
                case '\\': ostream << "\\\\"; break;
                case '\b': ostream << "\\b"; break;
                case '\f': ostream << "\\f"; break;
                case '\n': ostream << "\\n"; break;
                case '\r': ostream << "\\r"; break;
                case '\t': ostream << "\\t"; break;
                default: ostream.put(c); break;
            }
        }
    }

    void MaybeComma()
    {
        if (level_stack.back())
            ostream.put(',');
        else
            level_stack.back() = true;
    }


    std::ostream& ostream;
    std::vector<bool> level_stack;
};

template<typename OStream = std::ostream>
class PrettyWriter : public Writer<OStream>
{
public:
    explicit PrettyWriter(OStream& ostream, int indent = 4)
        : Writer<OStream>(ostream), indent_size(indent), current_indent(0)
    {
        in_array_stack.push_back(false);
    }

    void StartObject() override
    {
        this->MaybeComma();
        if (in_array_stack.back())
        {
            this->ostream.put('\n');
            WriteIndent();
        }
        this->ostream.put('{');
        current_indent += indent_size;
        in_array_stack.push_back(false);
        this->level_stack.push_back(false);
    }
    void FinishObject() override
    {
        current_indent -= indent_size;
        this->level_stack.pop_back();
        in_array_stack.pop_back();
        this->ostream.put('\n');
        WriteIndent();
        this->ostream.put('}');
    }
    void StartArray() override
    {
        this->MaybeComma();
        if (in_array_stack.back())
        {
            this->ostream.put('\n');
            WriteIndent();
        }
        this->ostream.put('[');
        current_indent += indent_size;
        in_array_stack.push_back(true);
        this->level_stack.push_back(false);
    }
    void FinishArray() override
    {
        current_indent -= indent_size;
        this->level_stack.pop_back();
        in_array_stack.pop_back();
        this->ostream.put('\n');
        WriteIndent();
        this->ostream.put(']');
    }
    void Key(std::string_view key) override
    {
        this->MaybeComma();
        this->ostream.put('\n');
        WriteIndent();
        this->ostream.put('"');
        this->WriteEscaped(key);
        this->ostream.put('"');
        this->ostream.put(':');
        this->ostream.put(' ');
        this->level_stack.back() = false;
    }

protected:
    void WriteIndent()
    {
        for (int i = 0; i < current_indent; ++i)
        {
            this->ostream.put(' ');
        }
    }

    int indent_size;
    int current_indent;
    std::vector<bool> in_array_stack;
};

template<typename Handler, typename IStream = std::istream>
class Reader
{
public:
    Reader(Handler& handler, IStream& istream)
        : handler(handler) , istream(istream)
    {
    }

    void Parse()
    {
        char ch;
        while (istream.get(ch))
        {
            switch (ch)
            {
                case '{':
                    handler.StartObject();
                    break;
                case '}':
                    handler.FinishObject();
                    break;
                case '[':
                    handler.StartArray();
                    break;
                case ']':
                    handler.FinishArray();
                    break;
                case '"':
                    handler.String(ReadString());
                    break;
                case ':':
                case ',':
                    break;
                default:
                    if (std::isdigit(ch) || ch == '-' || ch == '+')
                    {
                        istream.putback(ch);
                        handler.Number(ReadNumber());
                    }
                    else if (std::isalpha(ch))
                    {
                        istream.putback(ch);
                        handler.Keyword(ReadKeyword());
                    }
                    break;
            }
        }
    }

private:
    std::string ReadString()
    {
        std::string str;
        char ch;
        bool escape = false;
        while (istream.get(ch))
        {
            if (escape)
            {
                switch (ch)
                {
                    case '"': str.push_back('"'); break;
                    case '\\': str.push_back('\\'); break;
                    case '/': str.push_back('/'); break;
                    case 'b': str.push_back('\b'); break;
                    case 'f': str.push_back('\f'); break;
                    case 'n': str.push_back('\n'); break;
                    case 'r': str.push_back('\r'); break;
                    case 't': str.push_back('\t'); break;
                    default: str.push_back(ch); break;
                }
                escape = false;
            }
            else
            {
                if (ch == '\\')
                {
                    escape = true;
                }
                else if (ch == '"')
                {
                    break;
                }
                else
                {
                    str.push_back(ch);
                }
            }
        }
        return str;
    }

    std::string ReadNumber()
    {
        std::string num;
        char ch;
        while (istream.get(ch))
        {
            if (std::isdigit(ch) || ch == '.' || ch == 'e' || ch == 'E' || ch == '-' || ch == '+')
            {
                num.push_back(ch);
            }
            else
            {
                istream.putback(ch);
                break;
            }
        }
        return num;
    }

    std::string ReadKeyword()
    {
        std::string keyword;
        char ch;
        while (istream.get(ch))
        {
            if (std::isalpha(ch))
            {
                keyword.push_back(ch);
            }
            else
            {
                istream.putback(ch);
                break;
            }
        }
        return keyword;
    }

    Handler& handler;
    IStream& istream;
};
}

#endif
