<div align="center">

<a href="https://s2.loli.net/2024/11/01/uMOI5ctdaTNCb9R.png" target="_blank"><img src="https://s2.loli.net/2024/11/01/uMOI5ctdaTNCb9R.png"></a>

# JsOvOn

Another lightweight C++ JSON library, provided simply as a `C++ module`.

<p align="center">

<img src="https://img.shields.io/badge/C%2B%2B20-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white"> <img src="https://img.shields.io/github/license/arttnba3/JsOvOn?style=for-the-badge" alt="license">

</p>

</div>

## Usage

### Import the Library

To use the `JsOvOn` library, all we only need to do is to `"do the import"`:

```cpp
import jsovon;
```

Don't forget to add the source code of `JsOvOn` to your project and link the `JsOvOn` library into your `CMakeLists.txt`.

You can refer to `src/test.cpp` and `src/CMakeLists.txt` to see how to import and use this library.

> Currently you may need to enable CMake's support for `import std` manually, as it's still an experimental feature for some compilers like `clang++`.

### Use class `Json`

Simply, we can create the `Json` object for basic data type like this:

```cpp
jsovon::Json test;

/* number */
test = 123;
unsigned int a = test;
test = -456;
int b = test;
test = 789.1011F;
float c = test;
test = 1213.1415;
double d = test;

/* string */
test = "1617181920";
std::string e = test;

/* bool */
test = true;
bool f = test;

/* null */
test = jsovon::null;
if (test == jsovon::null) { /* true here */
    std::cout << "Hello world!" << std::endl;
}
```

For common json object type, it can be used like this:

```cpp
jsovon::Json object;

object = {
    { "key1", 123 },
    { "key2", -456 },
    { "key3", 789.1011F },
    { "key4", 1213.1415 },
    { "key5", "1617181920" },
    { "key6", true },
    { "key7", jsovon::null }
};

object["key8"] = object;

unsigned int a = object["key1"];
int b = object["key2"];
float c = object["key3"];
double d = object["key4"];
std::string e = object["key5"];
e = (std::string) object["key5"]; /* note that to re-assign a string val from Json object, use casting explicitly */
bool f = object["key6"];
jsovon::Json test = object["key7"];
```

For using the array type, just do it as follow:

```cpp
jsovon::Json array;

/* number */
array.append(123);
unsigned int a = array[0];
array.append(-456);
int b = array[1];
array.append(789.1011F);
float c = array[2];
array.append(1213.1415);
double d = array[3];

/* string */
array.append("1617181920");
std::string e = array[4];
e = (std::string) array[4]; /* note that to re-assign a string val from Json array, use casting explicitly */

/* bool */
array.append(true);
bool f = array[5];

/* null */
array.append(jsovon::null);
jsovon::Json g = array[6];
```

### Print class `Json` into JSON format

All we need to do is to call the `str()` method, or you can print it directly into a stream:

```cpp
jsovon::Json object = {
    { "key1", 123 },
    { "key2", -456 },
    { "key3", 789.1011F },
    { "key4", 1213.1415 },
    { "key5", "1617181920" },
    { "key6", true },
    { "key7", jsovon::null }
};

std::cout << object << std::endl;
std::cout << object.str() << std::endl;
```

### Parse the JSON text into class `Json`

To parse a string into a `Json` class, just simply do as follow:

```cpp
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

jsovon::Json test = jsovon::ParseJsonText(json_str);
```

Alternatively, you can also choose to parse a json file as follow:

```cpp
jsovon::Json test = ParseJsonFile("test.json");
```

## Author

- Code: arttnba3 <arttnba3@outlook.com>
- Illustration: Sumi Kiriko <kirikosumi@gmail.com>

## License

MIT
