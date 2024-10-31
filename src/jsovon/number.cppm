export module jsovon.number;

import jsovon.base;
import std;

namespace jsovon {

enum JsonNumberType {
    JSON_NUMBER_TYPE_BASE = 0,
    JSON_NUMBER_TYPE_INT,
    JSON_NUMBER_TYPE_UINT,
    JSON_NUMBER_TYPE_FLOAT,
    JSON_NUMBER_TYPE_NR,
};

export class JsonNumber : public JsonDataBase {
public:
    JsonNumber() { this->type = JSON_TYPE_NUMBER; }
    ~JsonNumber() { }

    virtual auto GetIntVal(void) -> std::int64_t = 0;
    virtual auto GetIntVal(void) const -> std::int64_t = 0;
    virtual auto GetUIntVal(void) -> std::uint64_t = 0;
    virtual auto GetUIntVal(void) const -> std::uint64_t = 0;
    virtual auto GetFloatVal(void) -> double = 0;
    virtual auto GetFloatVal(void) const -> double = 0;

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

    virtual auto str(void) const -> std::string = 0;
    virtual auto str_loop(std::uint64_t layer) const -> std::string;

protected:
    JsonNumberType num_type;
};

JsonNumber::operator std::int64_t ()
{
    return this->GetIntVal();
}

JsonNumber::operator std::int64_t () const
{
    return this->GetIntVal();
}

JsonNumber::operator std::uint64_t ()
{
    return this->GetUIntVal();
}

JsonNumber::operator std::uint64_t () const
{
    return this->GetUIntVal();
}

JsonNumber::operator std::int32_t ()
{
    return this->GetIntVal();
}

JsonNumber::operator std::int32_t () const
{
    return this->GetIntVal();
}

JsonNumber::operator std::uint32_t ()
{
    return this->GetUIntVal();
}

JsonNumber::operator std::uint32_t () const
{
    return this->GetUIntVal();
}

JsonNumber::operator std::int16_t ()
{
    return this->GetIntVal();
}

JsonNumber::operator std::int16_t () const
{
    return this->GetIntVal();
}

JsonNumber::operator std::uint16_t ()
{
    return this->GetUIntVal();
}

JsonNumber::operator std::uint16_t () const
{
    return this->GetUIntVal();
}

JsonNumber::operator std::int8_t ()
{
    return this->GetIntVal();
}

JsonNumber::operator std::int8_t () const
{
    return this->GetIntVal();
}

JsonNumber::operator std::uint8_t ()
{
    return this->GetUIntVal();
}

JsonNumber::operator std::uint8_t () const
{
    return this->GetUIntVal();
}

JsonNumber::operator float ()
{
    return this->GetFloatVal();
}

JsonNumber::operator float () const
{
    return this->GetFloatVal();
}

JsonNumber::operator double ()
{
    return this->GetFloatVal();
}

JsonNumber::operator double () const
{
    return this->GetFloatVal();
}

auto JsonNumber::str_loop(std::uint64_t layer) const -> std::string
{
    return this->str();
}

export class JsonNumberInt : public JsonNumber {
public:
    JsonNumberInt(std::int64_t val) : JsonNumber(), val(val) { this->num_type = JSON_NUMBER_TYPE_INT; }
    ~JsonNumberInt() { }

    virtual auto GetIntVal(void) -> std::int64_t;
    virtual auto GetIntVal(void) const -> std::int64_t;
    virtual auto GetUIntVal(void) -> std::uint64_t;
    virtual auto GetUIntVal(void) const -> std::uint64_t;
    virtual auto GetFloatVal(void) -> double;
    virtual auto GetFloatVal(void) const -> double;

    virtual auto Clone(void) const -> JsonDataBase*;

    virtual auto str(void) const -> std::string;

private:
    std::int64_t val;
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
};

auto JsonNumberInt::GetIntVal(void) -> std::int64_t
{
    return this->val;
}

auto JsonNumberInt::GetIntVal(void) const -> std::int64_t
{
    return this->val;
}

auto JsonNumberInt::GetUIntVal(void) -> std::uint64_t
{
    return static_cast<std::uint64_t>(this->val);
}

auto JsonNumberInt::GetUIntVal(void) const -> std::uint64_t
{
    return static_cast<std::uint64_t>(this->val);
}

auto JsonNumberInt::GetFloatVal(void) -> double
{
    return static_cast<double>(this->val);
}

auto JsonNumberInt::GetFloatVal(void) const -> double
{
    return static_cast<double>(this->val);
}

auto JsonNumberInt::Clone(void) const -> JsonDataBase*
{
    return new JsonNumberInt(this->val);
}

auto JsonNumberInt::InternalClone(const JsonDataBase &orig) -> void
{
    this->val = *dynamic_cast<const JsonNumberInt*>(&orig);
}

auto JsonNumberInt::str(void) const -> std::string
{
    return std::to_string(this->val);
}

export class JsonNumberUInt : public JsonNumber {
public:
    JsonNumberUInt(std::uint64_t val) : JsonNumber(), val(val) { }
    ~JsonNumberUInt() { }

    virtual auto GetIntVal(void) -> std::int64_t;
    virtual auto GetIntVal(void) const -> std::int64_t;
    virtual auto GetUIntVal(void) -> std::uint64_t;
    virtual auto GetUIntVal(void) const -> std::uint64_t;
    virtual auto GetFloatVal(void) -> double;
    virtual auto GetFloatVal(void) const -> double;

    virtual auto Clone(void) const -> JsonDataBase*;

    virtual auto str(void) const -> std::string;

private:
    std::uint64_t val;
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
};

auto JsonNumberUInt::GetIntVal(void) -> std::int64_t
{
    return static_cast<std::int64_t>(this->val);
}

auto JsonNumberUInt::GetIntVal(void) const -> std::int64_t
{
    return static_cast<std::int64_t>(this->val);
}

auto JsonNumberUInt::GetUIntVal(void) -> std::uint64_t
{
    return this->val;
}

auto JsonNumberUInt::GetUIntVal(void) const -> std::uint64_t
{
    return this->val;
}

auto JsonNumberUInt::GetFloatVal(void) -> double
{
    return static_cast<double>(this->val);
}

auto JsonNumberUInt::GetFloatVal(void) const -> double
{
    return static_cast<double>(this->val);
}

auto JsonNumberUInt::Clone(void) const -> JsonDataBase*
{
    return new JsonNumberUInt(this->val);
}

auto JsonNumberUInt::InternalClone(const JsonDataBase &orig) -> void
{
    this->val = *dynamic_cast<const JsonNumberUInt*>(&orig);
}

auto JsonNumberUInt::str(void) const -> std::string
{
    return std::to_string(this->val);
}

export class JsonNumberFloat : public JsonNumber {
public:
    JsonNumberFloat(double val) : JsonNumber(), val(val) { }
    ~JsonNumberFloat() { }

    virtual auto GetIntVal(void) -> std::int64_t;
    virtual auto GetIntVal(void) const -> std::int64_t;
    virtual auto GetUIntVal(void) -> std::uint64_t;
    virtual auto GetUIntVal(void) const -> std::uint64_t;
    virtual auto GetFloatVal(void) -> double;
    virtual auto GetFloatVal(void) const -> double;

    virtual auto Clone(void) const -> JsonDataBase*;

    virtual auto str(void) const -> std::string;

private:
    double val;
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
};

auto JsonNumberFloat::GetIntVal(void) -> std::int64_t
{
    return static_cast<std::int64_t>(this->val);
}

auto JsonNumberFloat::GetIntVal(void) const -> std::int64_t
{
    return static_cast<std::int64_t>(this->val);
}

auto JsonNumberFloat::GetUIntVal(void) -> std::uint64_t
{
    return static_cast<std::uint64_t>(this->val);
}

auto JsonNumberFloat::GetUIntVal(void) const -> std::uint64_t
{
    return static_cast<std::uint64_t>(this->val);
}

auto JsonNumberFloat::GetFloatVal(void) -> double
{
    return this->val;
}

auto JsonNumberFloat::GetFloatVal(void) const -> double
{
    return this->val;
}

auto JsonNumberFloat::Clone(void) const -> JsonDataBase*
{
    return new JsonNumberFloat(this->val);
}

auto JsonNumberFloat::InternalClone(const JsonDataBase &orig) -> void
{
    this->val = *dynamic_cast<const JsonNumberFloat*>(&orig);
}

auto JsonNumberFloat::str(void) const -> std::string
{
    /* it doesn't give me an accurate string :( */
    // this->num_str = std::to_string(val);
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

}
