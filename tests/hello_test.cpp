#include "src/hello.hpp"
#include "gtest/gtest.h"

TEST(GreeterTest, SayHello) {
  rest_api_helper::Greeter greeter("Bazel");
  EXPECT_EQ(greeter.say_hello(), "Hello, Bazel!");
}
