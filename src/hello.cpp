#include "src/hello.hpp"

#include <utility>

namespace rest_api_helper {

Greeter::Greeter(std::string name) : name_(std::move(name)) {}

std::string Greeter::say_hello() const { return "Hello, " + name_ + "!"; }

} // namespace rest_api_helper
