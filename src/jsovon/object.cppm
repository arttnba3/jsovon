export module jsovon.object;

import jsovon.base;
import std;

namespace jsovon {

/* temporarily we could only use this ugly way to implement it :( */

extern "C" auto __jsovon_internal_allocate_empty_json_object(void) -> void*;
extern "C" auto __jsovon_internal_delete_json_object(void *json_ptr) -> void;
extern "C" auto __jsovon_internal_copy_json_object_status(void *dst, void *src) -> void;
extern "C" auto __jsovon_internal_print_json_object(void *json_ptr, std::uint64_t layer) -> std::string;
extern "C" auto __jsovon_internal_get_json_type(void *json_ptr) -> JsonDataBaseType;

export class JsonObject : public JsonDataBase {
public:
    JsonObject() { this->type = JSON_TYPE_OBJECT; }
    ~JsonObject();

    virtual auto operator[](const std::string& key) -> void*&;

    virtual auto Clone(void) const -> JsonDataBase*;

    virtual auto str(void) const -> std::string;
    virtual auto str_loop(std::uint64_t layer) const -> std::string;

private:
    std::unordered_map<std::string, void*> map;
    virtual auto InternalClone(const JsonDataBase &orig) -> void;
    virtual auto GetInternalMap(void) const -> const std::unordered_map<std::string, void*>&;
    virtual auto append(std::string key, void *orig_json) -> void;
};

JsonObject::~JsonObject()
{
    for (auto itr = this->map.begin(); itr != this->map.end(); itr++) {
        __jsovon_internal_delete_json_object(this->map[itr->first]);
        this->map[itr->first] = nullptr;
    }
}

auto JsonObject::operator[](const std::string& key) -> void*&
{
    if (!this->map.count(key)) {
        this->map[key] = __jsovon_internal_allocate_empty_json_object();
    }

    return this->map[key];
}

auto JsonObject::Clone(void) const -> JsonDataBase*
{
    JsonObject *copied = new JsonObject();

    for (auto itr = this->map.begin(); itr != this->map.end(); itr++) {
        /**
         * NOTE: type (empty) is only an internal and intermediate type, ignore it
         * FIXIT: type (null) is an internal error, generally caused by assign an object to itself, ignore it until we fix it:(
         */
        JsonDataBaseType json_type = __jsovon_internal_get_json_type(itr->second);
        if (json_type == JSON_TYPE_NON_EXISTED || json_type == JSON_TYPE_EMPTY) {
            continue;
        }
        copied->append(itr->first, itr->second);
    }

    return dynamic_cast<JsonDataBase*>(copied);
}

auto JsonObject::InternalClone(const JsonDataBase &orig) -> void
{
    const std::unordered_map<std::string, void*> &orig_map = orig.GetInternalMap();

    this->map.clear();

    for (auto itr = orig_map.begin(); itr != orig_map.end(); itr++) {
        this->append(itr->first, itr->second);
    }
}

auto JsonObject::GetInternalMap(void) const -> const std::unordered_map<std::string, void*>&
{
    return this->map;
}

auto JsonObject::append(std::string key, void *orig_json) -> void
{
    void *new_json = __jsovon_internal_allocate_empty_json_object();
    __jsovon_internal_copy_json_object_status(new_json, orig_json);
    this->map[key] = new_json;
}

auto JsonObject::str(void) const -> std::string
{
    return this->str_loop(0);
}

auto JsonObject::str_loop(std::uint64_t layer) const -> std::string
{
    std::string res = "{", tab = "";
    auto itr = this->map.begin();

    for (auto i = 0; i < layer; i++) {
        tab += "\t";
    }

    if (this->map.size() == 0) {
        goto loop_end;
    }

    itr = this->map.begin();
    res += std::format("\n\t{}\"{}\" : {}", tab, itr->first, __jsovon_internal_print_json_object(itr->second, layer + 1));
    itr++;

    while (itr != this->map.end()) {
        res += std::format(",\n\t{}\"{}\" : {}", tab, itr->first, __jsovon_internal_print_json_object(itr->second, layer + 1));
        itr++;
    }

loop_end:
    res += "\n";
    res += tab;
    res += "}";

    return res;
}

};
