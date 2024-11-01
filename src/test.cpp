import jsovon;
import std;

auto functionality_test(void) -> void
{
    jsovon::Json json;
    std::cout << "Value: " << json << std::endl;

    json = 123;
    std::uint32_t val1 = json;
    std::cout << "Value: " << json << " " << val1 << std::endl;

    json = -456;
    std::int32_t val2 = json;
    std::cout << "Value: " << json << " " << val2 << std::endl;

    json = 789.1011F;
    float val3 = json;
    std::cout << "Value: " << json << " " << val3 << std::endl;

    json = 1213.1415;
    double val4 = json;
    std::cout << "Value: " << json << " " << val4 << std::endl;

    json = "1617181920";
    std::string val5 = json;
    std::cout << "Value: " << json << " " << val5 << std::endl;

    json = true;
    bool val6 = json;
    std::cout << "Value: " << json << " " << val6 << std::endl;

    jsovon::Json arr;

    arr.append(123);
    val1 = arr[0];
    std::cout << "arr[0]: " << arr[0] << " " << val1 << std::endl;

    arr.append(-456);
    val1 = arr[1];
    std::cout << "arr[1]: " << arr[1] << " " << val2 << std::endl;

    arr.append(789.1011F);
    val2 = arr[2];
    std::cout << "arr[2]: " << arr[2] << " " << val3 << std::endl;

    arr[0] = 1213.1415;
    val4 = arr[0];
    std::cout << "arr[0]: " << arr[0] << " " << val4 << std::endl;

    arr[1] = "1617181920";
    val5 = (std::string) arr[1];
    std::cout << "arr[1]: " << arr[1] << " " << val5 << std::endl;

    arr[2] = true;
    val6 = arr[2];
    std::cout << "arr[2]: " << arr[2] << " " << val6 << std::endl;

    arr.append(jsovon::null);
    std::cout << "arr[3] compared to jsovon::null is " << (arr[3] == jsovon::null) << std::endl;

    arr.append(arr);
    arr.append(&arr);

    std::cout << "arr: " << arr << std::endl;

    std::vector<jsovon::Json> jvec = {
        123, -456, 789.1011F, 1213.1415, "1617181920", true, jsovon::null
    };
    jsovon::Json arr2 = jvec;

    std::cout << "arr2: " << arr2 << std::endl;

    jsovon::Json obj;

    obj["0"] = 123;
    val1 = obj["0"];
    std::cout << "obj[\"0\"]: " << obj["0"] << " " << val1 << std::endl;

    obj["1"] = -456;
    val2 = obj["1"];
    std::cout << "obj[\"1\"]: " << obj["1"] << " " << val2 << std::endl;

    obj["2"] = 789.1011F;
    val3 = obj["2"];
    std::cout << "obj[\"2\"]: " << obj["2"] << " " << val3 << std::endl;

    obj["0"] = 1213.1415;
    val4 = obj["0"];
    std::cout << "obj[\"0\"]: " << obj["0"] << " " << val4 << std::endl;

    obj["1"] = "1617181920";
    val5 = (std::string) obj["1"];
    std::cout << "obj[\"1\"]: " << obj["1"] << " " << val5 << std::endl;

    obj["2"] = false;
    val6 = obj["2"];
    std::cout << "obj[\"2\"]: " << obj["2"] << " " << val6 << std::endl;

    obj["3"] = jsovon::null;
    std::cout << "obj[3] not compared to jsovon::null is " << (obj["3"] != jsovon::null) << std::endl;

    std::cout << "obj: " << obj << std::endl;

    jsovon::Json obj2 = {
        { "key1", 123 },
        { "key2", -456 },
        { "key3", 789.1011F },
        { "key4", 1213.1415 },
        { "key5", "1617181920" },
        { "key6", true },
        { "key7", obj },
        { "key8", &obj },
        { "key9", obj["3"] },
    };

    std::cout << "obj2: " << obj2 << std::endl;

    std::unordered_map<std::string, jsovon::Json> jmap = {
        { "key1", 123 },
        { "key2", -456 },
        { "key3", 789.1011F },
        { "key4", 1213.1415 },
        { "key5", "1617181920" },
        { "key6", true },
        { "key7", obj },
        { "key8", &obj },
        { "key9", obj["3"] },
    };

    jsovon::Json obj3 = jmap;

    std::cout << "obj3: " << obj3 << std::endl;
}

auto parsing_test(void) -> void
{
    auto json_str = R"(
        {
            "key1" : 123,
            "key2" : -456,
            "key3" : 789.101,
            "key4" : 1213.14,
            "key5" : "1617181920",
            "key6" : true,
            "key7" : null
        }
    )";

    std::cout << "Parse result 1: " << jsovon::ParseJsonFile("./test.json") << std::endl;
    std::cout << "Parse result 2:" << jsovon::ParseJsonText(json_str) << std::endl;
}

auto main(int argc, char **argv, char **envp) -> int
{
    functionality_test();
    parsing_test();

    return 0;
}
