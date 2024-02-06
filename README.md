# JSON-Parser

Project made entirely in C++

# JSON-Parser

![C++](https://img.shields.io/badge/language-C%2B%2B-orange) ![License](https://img.shields.io/badge/license-MIT-blue)

**JSON-Parser** is a C++ source file for parsing JSON-like data structures. It provides a versatile `json` class that can represent different JSON-like types using an enumeration, making it easy to work with JSON-like data in your C++ applications. This source file also includes a custom iterator for efficient traversal of JSON-like objects and arrays.

## Features

- **Versatile `json` Class**: The `json` class can represent various JSON-like types, including objects, arrays, strings, numbers, booleans, and null values, thanks to an enumeration system:
```cpp
enum type {
    jnull,
    jboolean,
    jnumber,
    jstring,
    jlist,
    jdict
};

struct json::impl{
    type t;
    double d;
    bool b;
    std::string s;

    //heads and tails
    lista_json head;
    lista_json tail;

    dizionario head_dict;
    dizionario tail_dict;
}
```

- **Custom Iterator**: The source file includes a custom iterator that simplifies the process of traversing JSON-like objects and arrays, allowing you to easily access and manipulate JSON-like data.

- **Lightweight and Efficient**

## Usage

To use the JSON-Parser source file in your C++ project, follow these steps:

1. Copy the `json_parser.cpp` and `json_parser.hpp` files to your project directory.

2. Include the necessary header in your C++ code:

   ```cpp
   #include "json_parser.hpp"
   ```

3. Create a `json` object and parse JSON-like data manually or reading another file:

   ```cpp
   //example of a list
   json j;
   j.set_list();
   j.push_back("first element");
   //it can also push back a nested list
   json nested;
   nested.set_list();
   nested.push_back("first nested element");
   nested.push_back(false);
   j.push_back(nested);

   //to read a file
   std::ifstream file("filename.json");
   file>>j;
   ```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
