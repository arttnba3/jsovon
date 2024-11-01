export module jsovon.wrapper;

import jsovon.array;
import jsovon.base;
import jsovon.boolean;
import jsovon.error;
import jsovon.null;
import jsovon.number;
import jsovon.object;
import jsovon.string;
import std;

namespace jsovon {

export class Json {
public:
    Json(void);
    Json(const Json &orig);

    Json(std::initializer_list<std::pair<const std::string, const Json>> init_list);

    template<typename T>
    Json(std::vector<T> &array);

    template<typename T>
    Json(std::unordered_map<std::string, T> &map);

    template<typename T>
    Json(T val);

    ~Json();

    auto operator[](int idx) -> Json&;
    auto operator[](const std::string &key) -> Json&;
    auto operator[](const char* key) -> Json&;

    template<typename T>
    auto operator==(const T &val) -> bool;

    template<typename T>
    auto operator!=(const T &val) -> bool;

    auto operator=(const JsonDataBase *data) -> Json&;
    auto operator=(const Json &orig) -> Json&;
    auto operator=(const Json *orig) -> Json&;
    auto operator=(std::int64_t val) -> Json&;
    auto operator=(std::uint64_t val) -> Json&;
    auto operator=(std::int32_t val) -> Json&;
    auto operator=(std::uint32_t val) -> Json&;
    auto operator=(std::int16_t val) -> Json&;
    auto operator=(std::uint16_t val) -> Json&;
    auto operator=(std::int8_t val) -> Json&;
    auto operator=(std::uint8_t val) -> Json&;
    auto operator=(float val) -> Json&;
    auto operator=(double val) -> Json&;
    auto operator=(bool val) -> Json&;
    auto operator=(const char *val) -> Json&;
    auto operator=(char *val) -> Json&;
    auto operator=(std::string val) -> Json&;

    template<typename T>
    operator T() const
    {
        return *this->data;
    }

    friend auto operator<<(std::ostream &output, const Json &json) -> std::ostream&
    {
        output << *(json.data);
        return output;
    }

    template<typename T>
    auto append(T val) -> void;

    template<typename T>
    auto append(std::string key, T val) -> void;

    auto str(void) const -> std::string;
    auto str_loop(std::uint64_t layer) const -> std::string;

    auto GetType(void) -> JsonDataBaseType;

    auto BeNull(void) -> void;

private:
    JsonDataBase *data;
    auto clear_data(void) -> void;
};

Json::Json(void)
{
    this->data = new JsonEmpty;
}

Json::Json(const Json &orig)
{
    this->data = orig.data->Clone();
}

Json::Json(std::initializer_list<std::pair<const std::string, const Json>> init_list)
{
    this->data = new JsonObject();

    for (auto itr = init_list.begin(); itr != init_list.end(); itr++) {
        (*this)[itr->first] = itr->second;
    }
}

template<typename T>
Json::Json(std::vector<T> &array)
{
    this->data = new JsonArray();

    for (auto i = 0; i < array.size(); i++) {
        this->append(array[i]);
    }
}

template<typename T>
Json::Json(std::unordered_map<std::string, T> &map)
{
    this->data = new JsonObject();

    for (auto itr = map.begin(); itr != map.end(); itr++) {
        (*this)[itr->first] = itr->second;
    } 
}

template<typename T>
Json::Json(T val)
{
    this->data = nullptr;
    *this = val;
}

Json::~Json()
{
    this->clear_data();
}

auto Json::operator[](int idx) -> Json&
{
    JsonDataBaseType type = this->data->GetType();
    Json *json;

    switch (type) {
    case JSON_TYPE_EMPTY:
        this->clear_data();
        this->data = new JsonArray();
    case JSON_TYPE_ARRAY:
        json = static_cast<Json*>((*this->data)[idx]);
        return *json;
    default:
        throw OperationNotAllowError(
            std::format("Json type \"{}\" could not provide [int] operator!", json_type_str[type])
        );
    }
}

auto Json::operator[](const std::string &key) -> Json&
{
    JsonDataBaseType type = this->data->GetType();
    Json *json;

    switch (type) {
    case JSON_TYPE_EMPTY:
        this->clear_data();
        this->data = new JsonObject();
    case JSON_TYPE_OBJECT:
        json = static_cast<Json*>((*this->data)[key]);
        return *json;
    default:
        throw OperationNotAllowError(
            std::format("Json type \"{}\" could not provide [std::string] operator!", json_type_str[type])
        );
    }
}

auto Json::operator[](const char* key) -> Json&
{
    return (*this)[std::string(key)];
}

/* for basic data type only */
template<typename T>
auto Json::operator==(const T &val) -> bool
{
    Json target(val);
    auto src_typ = this->GetType();
    auto dst_typ = target.GetType();

    if (src_typ < JSON_TYPE_STRING || dst_typ < JSON_TYPE_STRING || src_typ != dst_typ) {
        throw OperationNotAllowError(
            std::format(
                "Comparison between type \"{}\" and \"{}\" is not allowed!",
                json_type_str[src_typ],
                json_type_str[dst_typ])
        );
    }

    return this->str() == target.str();
}

template<typename T>
auto Json::operator!=(const T &val) -> bool
{
    return !(*this == val);
}

auto Json::operator=(const JsonDataBase *data) -> Json&
{
    this->clear_data();
    this->data = data->Clone();
    return *this;
}

auto Json::operator=(const Json &orig) -> Json&
{
    this->clear_data();
    this->data = orig.data->Clone();
    return *this;
}

auto Json::operator=(const Json *orig) -> Json&
{
    this->clear_data();
    this->data = orig->data->Clone();
    return *this;
}

auto Json::operator=(std::int64_t val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberInt(val);
    return *this;
}

auto Json::operator=(std::uint64_t val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberUInt(val);
    return *this;
}

auto Json::operator=(std::int32_t val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberInt(val);
    return *this;
}

auto Json::operator=(std::uint32_t val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberUInt(val);
    return *this;
}

auto Json::operator=(std::int16_t val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberInt(val);
    return *this;
}

auto Json::operator=(std::uint16_t val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberUInt(val);
    return *this;
}

auto Json::operator=(std::int8_t val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberInt(val);
    return *this;
}

auto Json::operator=(std::uint8_t val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberUInt(val);
    return *this;
}

auto Json::operator=(float val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberFloat(val);
    return *this;
}

auto Json::operator=(double val) -> Json&
{
    this->clear_data();
    this->data = new JsonNumberFloat(val);
    return *this;
}

auto Json::operator=(bool val) -> Json&
{
    this->clear_data();
    this->data = new JsonBool(val);
    return *this;
}

auto Json::operator=(const char *val) -> Json&
{
    this->clear_data();
    this->data = new JsonString(val);
    return *this;
}

auto Json::operator=(char *val) -> Json&
{
    this->clear_data();
    this->data = new JsonString(val);
    return *this;
}

auto Json::operator=(std::string val) -> Json&
{
    this->clear_data();
    this->data = new JsonString(val);
    return *this;
}

template<typename T>
auto Json::append(T val) -> void
{
    JsonDataBaseType type = this->data->GetType();
    Json new_val = val;

    switch (type) {
    case JSON_TYPE_EMPTY:
        this->clear_data();
        this->data = new JsonArray();
    case JSON_TYPE_ARRAY:
        this->data->append(&new_val);
        break;
    default:
        throw OperationNotAllowError(
            std::format("Json type \"{}\" could not provide [int] operator!", json_type_str[type])
        );
    }
}

template<typename T>
auto Json::append(std::string key, T val) -> void
{
    JsonDataBaseType type = this->data->GetType();
    Json new_val = val;

    switch (type) {
    case JSON_TYPE_EMPTY:
        this->clear_data();
        this->data = new JsonObject();
    case JSON_TYPE_OBJECT:
        this->data->append(key, &new_val);
        break;
    default:
        throw OperationNotAllowError(
            std::format("Json type \"{}\" could not provide [int] operator!", json_type_str[type])
        );
    }
}

auto Json::clear_data(void) -> void
{
    delete this->data;
    this->data = nullptr;
}

auto Json::str(void) const -> std::string
{
    return this->data->str();
}

auto Json::str_loop(std::uint64_t layer) const -> std::string
{
    return this->data->str_loop(layer);
}

auto Json::GetType(void) -> JsonDataBaseType
{
    if (!this->data) {
        return JSON_TYPE_NON_EXISTED;
    }

    return this->data->GetType();
}

/* I know it's ugly to do so, but no better way temporarily :( */
auto Json::BeNull(void) -> void
{
    this->clear_data();
    this->data = new JsonNull();
}

extern "C" auto __jsovon_internal_allocate_empty_json_object(void) -> void*
{
    return new Json();
}

extern "C" auto __jsovon_internal_delete_json_object(void *json_ptr) -> void
{
    Json *json = static_cast<Json*>(json_ptr);
    delete json;
}

extern "C" auto __jsovon_internal_copy_json_object_status(void *dst, void *src) -> void
{
    Json *dst_json = static_cast<Json*>(dst);
    Json *src_json = static_cast<Json*>(src);

    *dst_json = *src_json;
}

extern "C" auto __jsovon_internal_print_json_object(void *json_ptr, std::uint64_t layer) -> std::string
{
    return static_cast<Json*>(json_ptr)->str_loop(layer);
}

extern "C" auto __jsovon_internal_get_json_type(void *json_ptr) -> JsonDataBaseType
{
    return static_cast<Json*>(json_ptr)->GetType();
}

const JsonNull __INTERNAL_JSON_NULL;
export const Json JSON_NULL(&__INTERNAL_JSON_NULL);
export const Json null(&__INTERNAL_JSON_NULL);

};