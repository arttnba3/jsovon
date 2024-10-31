export module jsovon.base;

import jsovon.error;
import std;

export namespace jsovon {

enum JsonDataBaseType {
    JSON_TYPE_NON_EXISTED = 0,
    JSON_TYPE_EMPTY,
    JSON_TYPE_OBJECT,
    JSON_TYPE_ARRAY,
    JSON_TYPE_STRING,
    JSON_TYPE_NUMBER,
    JSON_TYPE_BOOL,
    JSON_TYPE_NULL,
    JSON_TYPE_NR,
};

std::string json_type_str[JSON_TYPE_NR] = {
    "(null)",
    "(empty)",
    "object",
    "array",
    "string",
    "number",
    "bool",
    "null"
};

class JsonBase;

class JsonDataBase {
public:
    JsonDataBase() { }
    virtual ~JsonDataBase() { }

    JsonDataBase(const JsonDataBase &orig);
    virtual auto operator=(const JsonDataBase &orig) -> JsonDataBase&;
    virtual auto Clone(void) const -> JsonDataBase* = 0;

    virtual auto operator[](std::uint64_t idx) -> void*&;
    virtual auto operator[](const std::string& key) -> void*&;

    virtual operator bool ();
    virtual operator bool () const;
    virtual operator std::string ();
    virtual operator std::string () const;
    virtual operator std::int64_t ();
    virtual operator std::int64_t () const;
    virtual operator std::uint64_t ();
    virtual operator std::uint64_t () const;
    virtual operator std::int32_t ();
    virtual operator std::int32_t () const;
    virtual operator std::uint32_t ();
    virtual operator std::uint32_t () const;
    virtual operator std::int16_t ();
    virtual operator std::int16_t () const;
    virtual operator std::uint16_t ();
    virtual operator std::uint16_t () const;
    virtual operator std::int8_t ();
    virtual operator std::int8_t () const;
    virtual operator std::uint8_t ();
    virtual operator std::uint8_t () const;
    virtual operator float ();
    virtual operator float () const;
    virtual operator double ();
    virtual operator double () const;

    friend auto operator<<(std::ostream &output, const JsonDataBase &json) -> std::ostream&
    {
        output << json.str();
        return output;
    }
    virtual auto str(void) const -> std::string = 0;
    /* only differ for container type (array & object) */
    virtual auto str_loop(std::uint64_t layer) const -> std::string = 0;

    auto GetType(void) -> JsonDataBaseType;

    virtual auto append(void *orig_json) -> void;
    virtual auto append(std::string key, void *orig_json) -> void;

    virtual auto GetInternalArray(void) const -> const std::vector<void*>&;
    virtual auto GetInternalMap(void) const -> const std::unordered_map<std::string, void*>&;

protected:
    JsonDataBaseType type;
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
};

JsonDataBase::JsonDataBase(const JsonDataBase &orig)
{
    if (this->type != orig.type) {
        throw TypeNotMatchError(
            std::format("Json type mismatch from {} copy to {}!", json_type_str[orig.type], json_type_str[this->type])
        );
    }

    this->InternalClone(orig);
}

auto JsonDataBase::operator=(const JsonDataBase &orig) -> JsonDataBase&
{
    if (&orig != this) {
        if (this->type != orig.type) {
            throw TypeNotMatchError(
                std::format(
                    "Json type mismatch from {} assign to {}!", json_type_str[orig.type],
                    json_type_str[this->type]
                )
            );
        }

        this->InternalClone(orig);
    }

    return *this;
}

auto JsonDataBase::InternalClone(const JsonDataBase &orig) -> void
{
    throw InternalError(
        std::format(
            "Json type \"{}\" does not support copy/assign constructor! Subclass not implemented?",
            json_type_str[this->type]
        )
    );
}

auto JsonDataBase::operator[](std::uint64_t idx) -> void*&
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide [int] operator!", json_type_str[this->type])
    );
}

auto JsonDataBase::operator[](const std::string& key) -> void*&
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide [string] operator!", json_type_str[this->type])
    );
}

JsonDataBase::operator bool ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide bool value!", json_type_str[this->type])
    );
}

JsonDataBase::operator bool () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide bool value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::string ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide string value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::string () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide string value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::int64_t ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide int64_t value!",json_type_str[this->type])
    );
}

JsonDataBase::operator std::int64_t () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide int64_t value!",json_type_str[this->type])
    );
}

JsonDataBase::operator std::uint64_t ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide uint64_t value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::uint64_t () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide uint64_t value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::int32_t ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide int32_t value!",json_type_str[this->type])
    );
}

JsonDataBase::operator std::int32_t () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide int32_t value!",json_type_str[this->type])
    );
}

JsonDataBase::operator std::uint32_t ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide uint32_t value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::uint32_t () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide uint32_t value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::int16_t ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide int16_t value!",json_type_str[this->type])
    );
}

JsonDataBase::operator std::int16_t () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide int16_t value!",json_type_str[this->type])
    );
}

JsonDataBase::operator std::uint16_t ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide uint16_t value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::uint16_t () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide uint16_t value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::int8_t ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide int8_t value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::int8_t () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide int8_t value!", json_type_str[this->type])
    );
}

JsonDataBase::operator std::uint8_t ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide uint8_t value!",json_type_str[this->type])
    );
}

JsonDataBase::operator std::uint8_t () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide uint8_t value!",json_type_str[this->type])
    );
}

JsonDataBase::operator float ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide float value!", json_type_str[this->type])
    );
}

JsonDataBase::operator float () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide float value!", json_type_str[this->type])
    );
}

JsonDataBase::operator double ()
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide double value!", json_type_str[this->type])
    );
}

JsonDataBase::operator double () const
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide double value!", json_type_str[this->type])
    );
}

auto JsonDataBase::GetType(void) -> JsonDataBaseType
{
    return this->type;
}

auto JsonDataBase::append(void *orig_json) -> void
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide append(json) function!", json_type_str[this->type])
    );
}

auto JsonDataBase::append(std::string key, void *orig_json) -> void
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide append(key, json) function!", json_type_str[this->type])
    );
}

auto JsonDataBase::GetInternalArray(void) const -> const std::vector<void*>&
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide internal array member!", json_type_str[this->type])
    );
}

auto JsonDataBase::GetInternalMap(void) const -> const std::unordered_map<std::string, void*>&
{
    throw OperationNotAllowError(
        std::format("Json type \"{}\" could not provide internal map member!", json_type_str[this->type])
    );
}

class JsonEmpty : public JsonDataBase {
public:
    JsonEmpty() { this->type = JSON_TYPE_EMPTY; }
    ~JsonEmpty() { }

    virtual auto str(void) const -> std::string;
    virtual auto str_loop(std::uint64_t layer) const -> std::string;

    virtual auto Clone(void) const -> JsonDataBase*;

private:
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
};

auto JsonEmpty::Clone(void) const -> JsonDataBase*
{
    return new JsonEmpty();
}

auto JsonEmpty::str(void) const -> std::string
{
    return "(empty)";
}

auto JsonEmpty::str_loop(std::uint64_t layer) const -> std::string
{
    return this->str();
}

auto JsonEmpty::InternalClone(const JsonDataBase &orig) -> void
{
    /* do nothing */
}

/*
class JsonBase {
public:
    JsonBase(void) { }
    JsonBase(const JsonBase &orig);
    ~JsonBase() { }

    virtual auto operator[](std::uint64_t idx) -> JsonBase& = 0;
    virtual auto operator[](const std::string& key) -> JsonBase& = 0;

    virtual auto operator=(const Json &orig) -> JsonBase& = 0;
    virtual auto operator=(std::int64_t val) -> JsonBase& = 0;
    virtual auto operator=(std::uint64_t val) -> JsonBase& = 0;
    virtual auto operator=(std::int32_t val) -> JsonBase& = 0;
    virtual auto operator=(std::uint32_t val) -> JsonBase& = 0;
    virtual auto operator=(std::int16_t val) -> JsonBase& = 0;
    virtual auto operator=(std::uint16_t val) -> JsonBase& = 0;
    virtual auto operator=(std::int8_t val) -> JsonBase& = 0;
    virtual auto operator=(std::uint8_t val) -> JsonBase& = 0;
    virtual auto operator=(float val) -> JsonBase& = 0;
    virtual auto operator=(double val) -> JsonBase& = 0;
    virtual auto operator=(bool val) -> JsonBase& = 0;
    virtual auto operator=(const char *val) -> JsonBase& = 0;
    virtual auto operator=(char *val) -> JsonBase& = 0;
    virtual auto operator=(std::string val) -> JsonBase& = 0;

private:

};
*/

};
