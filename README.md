# rest-api-helper

A simple helper library for C++ aimed at making the creation of REST APIs easier and cleaner. It is built on top of [cpp-httplib](https://github.com/yhirose/cpp-httplib) and [nlohmann/json](https://github.com/nlohmann/json).

## Features

- **Route Management**: Define routes and endpoints with a fluent API.
- **Auto-Documentation**: Automatically generates a JSON documentation endpoint for your API.
- **Parameter Descriptions**: Add descriptions for path parameters and request/response details.
- **Bazel Support**: Ready to be integrated into Bazel projects.

## Installation

This library is designed to be used with Bazel with Bzlmod enabled.

### 1. Add dependency to `MODULE.bazel`

Add the following to your `MODULE.bazel`:

```python
# Assuming you have this in a registry or local override
bazel_dep(name = "rest_api_helper", version = "0.0.1")
```

### 2. Add dependency to your `BUILD` file

In your `BUILD` or `BUILD.bazel` file, add the library to your target's `deps`:

```python
cc_binary(
    name = "my_app",
    srcs = ["main.cpp"],
    deps = [
        "@rest_api_helper//src:rest_api_helper",
    ],
)
```

## Usage

Here is a simple example of how to use the library:

```cpp
#include <httplib.h>
#include "src/rest_api.hpp"

int main()
{
    httplib::Server server;
    // Initialize the API helper with a base route "/api"
    yuki::web::RestAPI api(server, "/api");

    // Enable the documentation endpoint at "/api/docs"
    api.add_docs_endpoint("docs");

    // Add a route "/api/hello"
    auto& hello_route = api.add_route("hello", "A simple hello route");

    // Add a GET endpoint to the route
    hello_route.add_endpoint(
        yuki::web::HTTPMethod::HTTP_GET,
        [](const httplib::Request&, httplib::Response& res) {
            res.set_content("Hello, World!", "text/plain");
        },
        "Returns a hello message"
    );

    // Start the server
    server.listen("0.0.0.0", 8080);

    return 0;
}
```

See `examples/basic_usage.cpp` for a more complete example.

## Building and Testing

To build the library:

```bash
bazel build //src:rest_api_helper
```

To run the tests:

```bash
bazel test //tests:unit_tests
```

## Dependencies

- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- [nlohmann/json](https://github.com/nlohmann/json)
- [googletest](https://github.com/google/googletest) (for testing)
