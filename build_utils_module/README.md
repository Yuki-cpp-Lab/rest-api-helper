# Build Utils Module

This module provides common build utilities and macros for strict C++ development using Bazel.

## Features

- **Strict C++ Macros**: Wrappers around `cc_library`, `cc_binary`, and `cc_test` that enforce C++20 and warnings-as-errors.
- **Dependencies**: It declares dependencies on `bazel_clang_format` and `bazel_clang_tidy`, simplifying version management (though root module overrides may still be needed).

## Usage

### 1. Add dependency in `MODULE.bazel`

```starlark
bazel_dep(name = "build_utils", version = "0.0.1")
```

If you are using it locally:

```starlark
local_path_override(
    module_name = "build_utils",
    path = "path/to/build_utils",
)
```

### 2. Configure Toolchains (Required in Root Module)

Due to Bazel restrictions (`toolchains_llvm` must be used by the root module), you must configure the toolchain in your project's `MODULE.bazel`:

```starlark
bazel_dep(name = "toolchains_llvm", version = "1.6.0", dev_dependency = True)

llvm = use_extension("@toolchains_llvm//toolchain/extensions:llvm.bzl", "llvm", dev_dependency = True)
llvm.toolchain(
    llvm_version = "21.1.6",
)
use_repo(llvm, "llvm_toolchain")
register_toolchains("@llvm_toolchain//:all", dev_dependency = True)
```

### 3. Configure `.bazelrc`

To use `clang-format` and `clang-tidy` with the LLVM toolchain:

```bash
# Clang-tidy configuration
build:clang-tidy --aspects @bazel_clang_tidy//clang_tidy:clang_tidy.bzl%clang_tidy_aspect
build:clang-tidy --output_groups=report
build:clang-tidy --@bazel_clang_tidy//:clang_tidy_config=//:clang_tidy_config
build:clang-tidy --@bazel_clang_tidy//:clang_tidy_executable=@llvm_toolchain//:clang-tidy

# Clang-format configuration
build:clang-format-base --output_groups=report
build:clang-format-base --@bazel_clang_format//:config=//:clang-format-config
build:clang-format-base --@bazel_clang_format//:binary=@llvm_toolchain//:clang-format

# Check-only mode
build:clang-format --config=clang-format-base \
    --aspects=@bazel_clang_format//:defs.bzl%check_aspect

# Fix mode
build:clang-format-fix --config=clang-format-base \
    --aspects=@bazel_clang_format//:defs.bzl%fix_aspect \
    --use_action_cache=false
```

### 4. Use Strict Macros

In your `BUILD.bazel` files:

```starlark
load("@build_utils//:defs.bzl", "strict_cc_library", "strict_cc_binary", "strict_cc_test")

strict_cc_library(
    name = "my_lib",
    srcs = ["my_lib.cpp"],
    hdrs = ["my_lib.hpp"],
)
```
