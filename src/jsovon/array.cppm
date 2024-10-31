export module jsovon.array;

import jsovon.base;
import jsovon.boolean;
import jsovon.error;
import jsovon.number;
import jsovon.string;
import std;

namespace jsovon {

/* temporarily we could only use this ugly way to implement it :( */

extern "C" auto __jsovon_internal_allocate_empty_json_object(void) -> void*;
extern "C" auto __jsovon_internal_delete_json_object(void *json_ptr) -> void;
extern "C" auto __jsovon_internal_copy_json_object_status(void *dst, void *src) -> void;
extern "C" auto __jsovon_internal_print_json_object(void *json_ptr, std::uint64_t layer) -> std::string;

export class JsonArray : public JsonDataBase {
public:
    JsonArray(void) { this->type = JSON_TYPE_ARRAY; }
    ~JsonArray();

    virtual auto operator[](std::uint64_t idx) -> void*&;

    virtual auto Clone(void) const -> JsonDataBase*;

    virtual auto str(void) const -> std::string;
    virtual auto str_loop(std::uint64_t layer) const -> std::string;

private:
    std::vector<void*> array;
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
    virtual auto GetInternalArray(void) const -> const std::vector<void*>&;
    virtual auto append(void *orig_json) -> void;
};

JsonArray::~JsonArray()
{
    for (auto i = 0; i < this->array.size(); i++) {
        __jsovon_internal_delete_json_object(this->array[i]);
        this->array[i] = nullptr;
    }
}

auto JsonArray::operator[](std::uint64_t idx) -> void*&
{
    if (idx >= this->array.size()) {
        throw IndexOutOfRangeError(std::format("Index \"{}\" is out of range!", idx));
    }

    return this->array[idx];
}

auto JsonArray::Clone(void) const -> JsonDataBase*
{
    JsonArray *copied = new JsonArray();

    for (auto i = 0; i < this->array.size(); i++) {
        copied->append(this->array[i]);
    }

    return dynamic_cast<JsonDataBase*>(copied);
}

auto JsonArray::InternalClone(const JsonDataBase &orig) -> void
{
    const std::vector<void*> &orig_array = orig.GetInternalArray();

    this->array.clear();

    for (auto i = 0; i < orig_array.size(); i++) {
        this->append(orig_array[i]);
    }
}

auto JsonArray::GetInternalArray(void) const -> const std::vector<void*>&
{
    return this->array;
}

auto JsonArray::append(void *orig_json) -> void
{
    void *new_json = __jsovon_internal_allocate_empty_json_object();
    __jsovon_internal_copy_json_object_status(new_json, orig_json);
    this->array.emplace_back(new_json);
}

auto JsonArray::str(void) const -> std::string
{
    return this->str_loop(0);
}

auto JsonArray::str_loop(std::uint64_t layer) const -> std::string
{
    std::string res = "[", tab;

    for (auto i = 0; i < layer; i++) {
        tab += "\t";
    }

    if (this->array.size() == 0) {
        goto loop_end;
    }

    res += "\n\t";
    res += tab;
    res += __jsovon_internal_print_json_object(this->array[0], layer + 1);

    for (auto i = 1; i < this->array.size(); i++) {
        res += ",\n\t";
        res += tab;
        res += __jsovon_internal_print_json_object(this->array[i], layer + 1);
    }

loop_end:
    res += "\n";
    res += tab;
    res += "]";

    return res;
}

}
