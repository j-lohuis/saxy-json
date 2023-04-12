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
}

#endif
