export module jsovon.boolean;

import jsovon.base;
import std;

namespace jsovon {

export class JsonBool : public JsonDataBase {
public:
    JsonBool(bool val) : val(val) { this->type = JSON_TYPE_BOOL; }
    ~JsonBool() { }

    virtual operator bool ();
    virtual operator bool () const;

    virtual auto str(void) const -> std::string;
    virtual auto str_loop(std::uint64_t layer) const -> std::string;

    virtual auto Clone(void) const -> JsonDataBase*;

private:
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
    bool val;
};

JsonBool::operator bool ()
{
    return this->val;
}

JsonBool::operator bool () const
{
    return this->val;
}

auto JsonBool::Clone(void) const -> JsonDataBase*
{
    return new JsonBool(this->val);
}

auto JsonBool::str(void) const -> std::string
{
    return this->val ? "true" : "false";
}

auto JsonBool::str_loop(std::uint64_t layer) const -> std::string
{
    return this->str();
}

auto JsonBool::InternalClone(const JsonDataBase &orig) -> void
{
    this->val = *dynamic_cast<const JsonBool*>(&orig);
}

}