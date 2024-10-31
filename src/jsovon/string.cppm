export module jsovon.string;

import jsovon.base;
import std;

namespace jsovon {

export class JsonString : public JsonDataBase {
public:
    JsonString(std::string val) : val(val) { this->type = JSON_TYPE_STRING; }
    ~JsonString() { }

    virtual operator std::string ();
    virtual operator std::string () const;

    virtual auto str(void) const -> std::string;
    virtual auto str_loop(std::uint64_t layer) const -> std::string;

    virtual auto Clone(void) const -> JsonDataBase*;

private:
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
    std::string val;
};

JsonString::operator std::string ()
{
    return this->val;
}

JsonString::operator std::string () const
{
    return this->val;
}

auto JsonString::str(void) const -> std::string
{
    return std::format("\"{}\"", this->val);
}

auto JsonString::str_loop(std::uint64_t layer) const -> std::string
{
    return this->str();
}

auto JsonString::Clone(void) const -> JsonDataBase*
{
    return new JsonString(this->val);
}

auto JsonString::InternalClone(const JsonDataBase &orig) -> void
{
    this->val = dynamic_cast<const JsonString*>(&orig)->val;
}

}
