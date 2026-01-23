# Rest API Helper

A C++ library to simplify the creation of REST APIs using `cpp-httplib`, featuring automatic documentation generation.

## Features

- **Route Management**: Organize routes and endpoints hierarchically.
- **Auto-Documentation**: Automatically generates a JSON schema of your API endpoints.
- **Easy Integration**: Wraps `httplib::Server` for seamless integration.

## Installation

This project uses Bazel for building and dependency management.

### Dependency

Add `rest_api_helper` to your `MODULE.bazel`. If you are using it locally:

```starlark
bazel_dep(name = "rest_api_helper", version = "0.0.1")
local_path_override(
    module_name = "rest_api_helper",
    path = "path/to/rest_api_helper",
)
```

## Usage

```cpp
#include "src/rest_api.hpp"
#include <httplib.h>

int main() {
    httplib::Server svr;
    yuki::web::RestAPI api(svr, "/api/v1");

    // Add a route
    auto& users_route = api.add_route("users", "Operations on users");

    // Add an endpoint to the route
    users_route.add_endpoint(
        yuki::web::HTTPMethod::HTTP_GET,
        [](const httplib::Request& req, httplib::Response& res) {
            res.set_content("List of users", "text/plain");
        },
        "Get all users"
    );

    // Add documentation endpoint
    api.add_docs_endpoint("docs");

    svr.listen("0.0.0.0", 8080);
    return 0;
}
```

## Build System

This project is built with Bazel and relies on a separate `build_utils` module for toolchain configuration and strict compilation flags.

### Modules

- **rest_api_helper**: The main library.
- **build_utils**: Provides toolchains (LLVM) and strict build macros.

### Building

```bash
bazel build //...
```

### Testing

```bash
bazel test //...
```

### Formatting

To automatically format the code using the project's configuration:

```bash
bazel build //... --config=clang-format-fix
```
