export module jsovon.null;

import jsovon.base;
import std;

namespace jsovon {

export class JsonNull : public JsonDataBase {
public:
    JsonNull(void) { this->type = JSON_TYPE_NULL; }
    ~JsonNull() { }

    virtual auto str(void) const -> std::string;
    virtual auto str_loop(std::uint64_t layer) const -> std::string;

    virtual auto Clone(void) const -> JsonDataBase*;

private:
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
};

auto JsonNull::Clone(void) const -> JsonDataBase*
{
    return new JsonNull();
}

auto JsonNull::str(void) const -> std::string
{
    return "null";
}

auto JsonNull::str_loop(std::uint64_t layer) const -> std::string
{
    return this->str();
}

auto JsonNull::InternalClone(const JsonDataBase &orig) -> void
{
    this->type = JSON_TYPE_NULL;
}

}
