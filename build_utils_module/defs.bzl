load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

_LINUX_FLAGS = [
    "-std=c++20",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Werror",
]

_WINDOWS_FLAGS = [
    "/std:c++20",
    "/WX",
    "/W4",
]

def _strict_copts():
    return select({
        "@platforms//os:linux": _LINUX_FLAGS,
        "@platforms//os:windows": _WINDOWS_FLAGS,
        "//conditions:default": _LINUX_FLAGS,
    })

def strict_cc_library(name, copts = [], **kwargs):
    """Wrapper around cc_library that enforces C++20 and warnings as errors."""
    cc_library(
        name = name,
        copts = _strict_copts() + copts,
        **kwargs
    )

def strict_cc_binary(name, copts = [], **kwargs):
    """Wrapper around cc_binary that enforces C++20 and warnings as errors."""
    cc_binary(
        name = name,
        copts = _strict_copts() + copts,
        **kwargs
    )

def strict_cc_test(name, copts = [], **kwargs):
    """Wrapper around cc_test that enforces C++20 and warnings as errors."""
    cc_test(
        name = name,
        copts = _strict_copts() + copts,
        **kwargs
    )
