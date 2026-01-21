#include <iostream>

#include "src/hello.hpp"

int main()
{
    rest_api_helper::Greeter greeter("World");
    std::cout << greeter.say_hello() << std::endl;
    return 0;
}
