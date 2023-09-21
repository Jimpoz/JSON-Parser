# JSON-Parser

Project made entirely in C++

# JSON-Parser

![C++](https://img.shields.io/badge/language-C%2B%2B-orange) ![License](https://img.shields.io/badge/license-MIT-blue)

**JSON-Parser** is a C++ source file for parsing JSON-like data structures. It provides a versatile `json` class that can represent different JSON-like types using an enumeration, making it easy to work with JSON-like data in your C++ applications. This source file also includes a custom iterator for efficient traversal of JSON-like objects and arrays.

## Features

- **Versatile `json` Class**: The `json` class can represent various JSON-like types, including objects, arrays, strings, numbers, booleans, and null values, thanks to an enumeration system.

- **Custom Iterator**: The source file includes a custom iterator that simplifies the process of traversing JSON-like objects and arrays, allowing you to easily access and manipulate JSON-like data.

- **Lightweight and Efficient**: The JSON-Parser source file is designed to be lightweight and efficient, making it suitable for use in resource-constrained environments.

## Usage

To use the JSON-Parser source file in your C++ project, follow these steps:

1. Copy the `json_parser.cpp` and `json_parser.h` files to your project directory.

2. Include the necessary header in your C++ code:

   ```cpp
   #include "json_parser.h"
   ```

3. Create a `json` object and parse JSON-like data:

   ```cpp
   json data = json::parse("{\"name\": \"John\", \"age\": 30}");
   ```

4. Use the `json` object and custom iterators to work with JSON-like data:

   ```cpp
   std::string name = data["name"].get<std::string>();
   int age = data["age"].get<int>();
   ```

5. Build and run your C++ project.

For more detailed usage examples and documentation, please refer to the [Wiki](https://github.com/yourusername/JSON-Parser/wiki).

## Examples

```cpp
#include <iostream>
#include "json_parser.h"

int main() {
    // Parse JSON-like data
    json data = json::parse("{\"name\": \"John\", \"age\": 30}");

    // Access and print values
    std::string name = data["name"].get<std::string>();
    int age = data["age"].get<int>();

    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;

    return 0;
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

If you would like to contribute to this project, please follow our [Contribution Guidelines](CONTRIBUTING.md).

## Acknowledgments

- This project was inspired by the need for a lightweight and efficient JSON-like parser in C++.
- Special thanks to the C++ community for their support and contributions.

## Contact

For any questions or feedback, please contact [your@email.com].

---

**Note**: Replace `[yourusername]` with your GitHub username and `[your@email.com]` with your contact email address in the provided links and contact information.

Enjoy using the JSON-Parser source file in your C++ projects!
